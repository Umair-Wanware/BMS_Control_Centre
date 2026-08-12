#include "drivers/wifi/wifi_manager.hpp"

#include <string.h>

#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "lwip/inet.h"

namespace control_hub::drivers::wifi {
namespace {

const char* TAG = "WiFiManager";

bool isLengthValid(const char* value, const size_t minimum, const size_t maximum) noexcept
{
    if (value == nullptr) {
        return minimum == 0U;
    }

    const size_t length = strnlen(value, maximum + 1U);
    return length >= minimum && length <= maximum;
}

void copyText(uint8_t* destination, const size_t capacity, const char* source) noexcept
{
    memset(destination, 0, capacity);
    if (source != nullptr) {
        const size_t length = strnlen(source, capacity - 1U);
        memcpy(destination, source, length);
    }
}

} // namespace

WiFiManager::WiFiManager()
{
    m_mutex = xSemaphoreCreateMutex();
}

WiFiManager::~WiFiManager()
{
    stop();
    if (m_wifiHandlerInstance != nullptr) {
        esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                              m_wifiHandlerInstance);
    }
    if (m_ipHandlerInstance != nullptr) {
        esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                              m_ipHandlerInstance);
    }
    if (m_mutex != nullptr) {
        vSemaphoreDelete(m_mutex);
    }
}

esp_err_t WiFiManager::initialize()
{
    if (m_initialized) {
        return ESP_OK;
    }
    if (m_mutex == nullptr) {
        return ESP_ERR_NO_MEM;
    }

    esp_err_t result = esp_netif_init();
    if (result != ESP_OK && result != ESP_ERR_INVALID_STATE) {
        return result;
    }

    result = esp_event_loop_create_default();
    if (result != ESP_OK && result != ESP_ERR_INVALID_STATE) {
        return result;
    }

    wifi_init_config_t wifiInitialization = WIFI_INIT_CONFIG_DEFAULT();
    result = esp_wifi_init(&wifiInitialization);
    if (result != ESP_OK && result != ESP_ERR_INVALID_STATE) {
        return result;
    }

    result = esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                                 &WiFiManager::wifiEventHandler, this,
                                                 &m_wifiHandlerInstance);
    if (result != ESP_OK) {
        return result;
    }

    result = esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                                 &WiFiManager::ipEventHandler, this,
                                                 &m_ipHandlerInstance);
    if (result != ESP_OK) {
        esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                              m_wifiHandlerInstance);
        m_wifiHandlerInstance = nullptr;
        return result;
    }

    m_initialized = true;
    return ESP_OK;
}

esp_err_t WiFiManager::startStation(const StationConfig& configuration)
{
    if (!isLengthValid(configuration.ssid, 1U, 32U) ||
        !isLengthValid(configuration.password, 0U, 64U)) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t result = initialize();
    if (result != ESP_OK) {
        return result;
    }

    if (m_stationNetif == nullptr) {
        m_stationNetif = esp_netif_create_default_wifi_sta();
        if (m_stationNetif == nullptr) {
            return ESP_ERR_NO_MEM;
        }
    }

    stop();

    wifi_config_t wifiConfiguration{};
    copyText(wifiConfiguration.sta.ssid, sizeof(wifiConfiguration.sta.ssid), configuration.ssid);
    copyText(wifiConfiguration.sta.password, sizeof(wifiConfiguration.sta.password),
             configuration.password);
    wifiConfiguration.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    wifiConfiguration.sta.pmf_cfg.capable = true;
    wifiConfiguration.sta.pmf_cfg.required = false;

    result = esp_wifi_set_mode(WIFI_MODE_STA);
    if (result == ESP_OK) {
        result = esp_wifi_set_config(WIFI_IF_STA, &wifiConfiguration);
    }
    if (result == ESP_OK) {
        result = esp_wifi_start();
    }
    if (result != ESP_OK) {
        return result;
    }

    if (takeLock()) {
        m_mode = Mode::Station;
        m_connected = false;
        m_reconnectAttempts = 0U;
        m_maximumReconnectAttempts = configuration.maximumReconnectAttempts;
        m_ipAddress[0] = '\0';
        giveLock();
    }

    return esp_wifi_connect();
}

esp_err_t WiFiManager::startAccessPoint(const AccessPointConfig& configuration)
{
    const size_t passwordMinimum = configuration.password == nullptr || configuration.password[0] == '\0'
                                       ? 0U
                                       : 8U;
    if (!isLengthValid(configuration.ssid, 1U, 32U) ||
        !isLengthValid(configuration.password, passwordMinimum, 63U) ||
        configuration.channel == 0U || configuration.channel > 13U ||
        configuration.maxConnections == 0U) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t result = initialize();
    if (result != ESP_OK) {
        return result;
    }

    if (m_accessPointNetif == nullptr) {
        m_accessPointNetif = esp_netif_create_default_wifi_ap();
        if (m_accessPointNetif == nullptr) {
            return ESP_ERR_NO_MEM;
        }
    }

    stop();

    wifi_config_t wifiConfiguration{};
    copyText(wifiConfiguration.ap.ssid, sizeof(wifiConfiguration.ap.ssid), configuration.ssid);
    copyText(wifiConfiguration.ap.password, sizeof(wifiConfiguration.ap.password),
             configuration.password);
    wifiConfiguration.ap.ssid_len = strlen(configuration.ssid);
    wifiConfiguration.ap.channel = configuration.channel;
    wifiConfiguration.ap.max_connection = configuration.maxConnections;
    wifiConfiguration.ap.authmode = passwordMinimum == 0U ? WIFI_AUTH_OPEN : WIFI_AUTH_WPA2_PSK;

    result = esp_wifi_set_mode(WIFI_MODE_AP);
    if (result == ESP_OK) {
        result = esp_wifi_set_config(WIFI_IF_AP, &wifiConfiguration);
    }
    if (result == ESP_OK) {
        result = esp_wifi_start();
    }
    if (result != ESP_OK) {
        return result;
    }

    if (takeLock()) {
        m_mode = Mode::AccessPoint;
        m_connected = false;
        m_ipAddress[0] = '\0';
        giveLock();
    }

    esp_netif_ip_info_t ipInfo{};
    if (m_accessPointNetif != nullptr && esp_netif_get_ip_info(m_accessPointNetif, &ipInfo) == ESP_OK) {
        ESP_LOGI(TAG, "AP started SSID=%s  IP=http://" IPSTR "/",
                 configuration.ssid, IP2STR(&ipInfo.ip));
    } else {
        ESP_LOGI(TAG, "AP started SSID=%s  IP=http://192.168.4.1/", configuration.ssid);
    }

    return ESP_OK;
}

esp_err_t WiFiManager::stop()
{
    if (!m_initialized) {
        return ESP_OK;
    }

    const esp_err_t result = esp_wifi_stop();
    if (takeLock()) {
        m_mode = Mode::Stopped;
        m_connected = false;
        m_reconnectAttempts = 0U;
        m_ipAddress[0] = '\0';
        giveLock();
    }
    return result == ESP_ERR_WIFI_NOT_INIT || result == ESP_ERR_WIFI_NOT_STARTED ? ESP_OK : result;
}

bool WiFiManager::isConnected() const noexcept
{
    bool connected = false;
    if (takeLock()) {
        connected = m_connected;
        giveLock();
    }
    return connected;
}

bool WiFiManager::isAccessPointActive() const noexcept
{
    bool accessPoint = false;
    if (takeLock()) {
        accessPoint = m_mode == Mode::AccessPoint;
        giveLock();
    }
    return accessPoint;
}

std::string WiFiManager::ipAddress() const
{
    std::string address;
    if (takeLock()) {
        address = m_ipAddress;
        giveLock();
    }
    return address;
}

void WiFiManager::wifiEventHandler(void* argument, esp_event_base_t, const int32_t eventId, void*)
{
    static_cast<WiFiManager*>(argument)->handleWifiEvent(eventId);
}

void WiFiManager::ipEventHandler(void* argument, esp_event_base_t, const int32_t eventId,
                                 void* eventData)
{
    static_cast<WiFiManager*>(argument)->handleIpEvent(eventId, eventData);
}

void WiFiManager::handleWifiEvent(const int32_t eventId)
{
    if (eventId == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
        return;
    }

    if (eventId != WIFI_EVENT_STA_DISCONNECTED || !takeLock()) {
        return;
    }

    m_connected = false;
    m_ipAddress[0] = '\0';
    const bool shouldReconnect = m_mode == Mode::Station &&
                                 m_reconnectAttempts < m_maximumReconnectAttempts;
    if (shouldReconnect) {
        ++m_reconnectAttempts;
    }
    giveLock();

    if (shouldReconnect) {
        esp_wifi_connect();
    }
}

void WiFiManager::handleIpEvent(const int32_t eventId, void* eventData)
{
    if (eventId != IP_EVENT_STA_GOT_IP || eventData == nullptr || !takeLock()) {
        return;
    }

    const auto* event = static_cast<const ip_event_got_ip_t*>(eventData);
    inet_ntoa_r(event->ip_info.ip, m_ipAddress, sizeof(m_ipAddress));
    m_connected = true;
    m_reconnectAttempts = 0U;
    giveLock();

    ESP_LOGI(TAG, "Station connected IP=http://" IPSTR "/", IP2STR(&event->ip_info.ip));
}

bool WiFiManager::takeLock() const noexcept
{
    return m_mutex != nullptr && xSemaphoreTake(m_mutex, pdMS_TO_TICKS(1000U)) == pdTRUE;
}

void WiFiManager::giveLock() const noexcept
{
    xSemaphoreGive(m_mutex);
}

} // namespace control_hub::drivers::wifi
