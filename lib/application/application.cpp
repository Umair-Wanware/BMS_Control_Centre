#include "application/application.hpp"

#include "nvs_flash.h"

#include "communication/commands.hpp"

namespace control_hub::application {

Application::Application()
    : m_fileSystem(), m_storage(m_fileSystem), m_wifi(), m_spi(), m_communication(m_spi),
      m_telemetry(), m_firmwareUpdater(m_communication, m_fileSystem),
      m_dashboard(&m_telemetry)
{
}

Application::~Application()
{
    if (m_telemetryTask != nullptr) {
        vTaskDelete(m_telemetryTask);
        m_telemetryTask = nullptr;
    }
}

esp_err_t Application::initialize()
{
    if (m_initialized) {
        return ESP_OK;
    }

    esp_err_t result = initializeNvs();
    if (result != ESP_OK) {
        return result;
    }

    result = m_fileSystem.initialize();
    if (result != ESP_OK) {
        m_logger.error("LittleFS initialization failed: %s", esp_err_to_name(result));
        return result;
    }

    result = startNetwork();
    if (result != ESP_OK) {
        m_logger.error("Wi-Fi initialization failed: %s", esp_err_to_name(result));
        return result;
    }

    result = m_spi.initialize();
    if (result != ESP_OK) {
        m_logger.error("SPI initialization failed: %s", esp_err_to_name(result));
        return result;
    }

    result = m_communication.initialize();
    if (result != ESP_OK) {
        m_logger.error("Communication manager initialization failed: %s", esp_err_to_name(result));
        return result;
    }

    if (!m_dashboard.Init()) {
        m_logger.error("Dashboard initialization failed");
        return ESP_FAIL;
    }

    if (xTaskCreate(&Application::telemetryTaskEntry, "telemetry", 4096U, this, 4U,
                    &m_telemetryTask) != pdPASS) {
        return ESP_ERR_NO_MEM;
    }

    m_initialized = true;
    m_logger.info("Control Hub initialized");
    return ESP_OK;
}

void Application::run() const
{
    while (true) {
        vTaskDelay(portMAX_DELAY);
    }
}

void Application::telemetryTaskEntry(void* argument)
{
    static_cast<Application*>(argument)->telemetryTask();
}

void Application::telemetryTask()
{
    while (true) {
        const esp_err_t requestResult = m_communication.sendCommandWithAcknowledgement(
            communication::Command::RequestTelemetry, nullptr, 0U);
        if (requestResult == ESP_OK) {
            communication::Packet packet{};
            const esp_err_t receiveResult = m_communication.receivePacket(packet);
            if (receiveResult == ESP_OK) {
                const esp_err_t telemetryResult = m_telemetry.processPacket(packet);
                if (telemetryResult != ESP_OK) {
                    m_logger.warn("Rejected telemetry packet: %s", esp_err_to_name(telemetryResult));
                }
            } else {
                m_logger.debug("Telemetry receive failed: %s", esp_err_to_name(receiveResult));
            }
        } else {
            m_logger.debug("Telemetry request failed: %s", esp_err_to_name(requestResult));
        }

        vTaskDelay(pdMS_TO_TICKS(1000U));
    }
}

esp_err_t Application::initializeNvs() const
{
    esp_err_t result = nvs_flash_init();
    if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        result = nvs_flash_erase();
        if (result == ESP_OK) {
            result = nvs_flash_init();
        }
    }
    return result;
}

esp_err_t Application::startNetwork()
{
    storage::NetworkSettings settings{};
    const esp_err_t settingsResult = m_storage.loadNetworkSettings(settings);
    if (settingsResult != ESP_OK) {
        m_logger.warn("Settings unavailable; starting access point: %s",
                      esp_err_to_name(settingsResult));
    }

    if (settingsResult == ESP_OK && settings.useStationMode) {
        const drivers::wifi::WiFiManager::StationConfig stationConfig{
            settings.stationSsid, settings.stationPassword, 10U};
        const esp_err_t stationResult = m_wifi.startStation(stationConfig);
        if (stationResult == ESP_OK) {
            return ESP_OK;
        }
        m_logger.warn("Station startup failed; falling back to access point: %s",
                      esp_err_to_name(stationResult));
    }

    const drivers::wifi::WiFiManager::AccessPointConfig accessPointConfig{
        settings.accessPointSsid, settings.accessPointPassword, 4U, 1U};
    return m_wifi.startAccessPoint(accessPointConfig);
}

} // namespace control_hub::application
