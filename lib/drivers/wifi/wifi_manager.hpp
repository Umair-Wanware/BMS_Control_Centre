#pragma once

#include <stdint.h>

#include <string>

#include "esp_err.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/** @file wifi_manager.hpp @brief ESP-IDF station and access-point lifecycle manager. */

namespace control_hub::drivers::wifi {

/**
 * @brief Manages Wi-Fi connection, reconnection, station mode, and AP mode.
 */
class WiFiManager final {
public:
    /** @brief Wi-Fi station credentials and reconnect policy. */
    struct StationConfig {
        const char* ssid = nullptr;
        const char* password = nullptr;
        uint8_t maximumReconnectAttempts = 10U;
    };

    /** @brief Soft access-point settings. */
    struct AccessPointConfig {
        const char* ssid = "BMS-Control-Hub";
        const char* password = nullptr;
        uint8_t maxConnections = 4U;
        uint8_t channel = 1U;
    };

    WiFiManager();
    ~WiFiManager();

    WiFiManager(const WiFiManager&) = delete;
    WiFiManager& operator=(const WiFiManager&) = delete;

    /** Initializes the ESP-IDF Wi-Fi and event infrastructure once. */
    esp_err_t initialize();

    /** Starts station mode and initiates a connection attempt. */
    esp_err_t startStation(const StationConfig& configuration);

    /** Starts soft access-point mode. */
    esp_err_t startAccessPoint(const AccessPointConfig& configuration);

    /** Stops the active Wi-Fi interface. */
    esp_err_t stop();

    /** Returns whether station mode currently has an IP address. */
    bool isConnected() const noexcept;

    /** Returns whether access-point mode is active. */
    bool isAccessPointActive() const noexcept;

    /** Returns the station IPv4 address, or an empty string before connection. */
    std::string ipAddress() const;

private:
    enum class Mode : uint8_t { Stopped, Station, AccessPoint };

    static void wifiEventHandler(void* argument, esp_event_base_t eventBase,
                                 int32_t eventId, void* eventData);
    static void ipEventHandler(void* argument, esp_event_base_t eventBase,
                               int32_t eventId, void* eventData);
    void handleWifiEvent(int32_t eventId);
    void handleIpEvent(int32_t eventId, void* eventData);
    bool takeLock() const noexcept;
    void giveLock() const noexcept;

    SemaphoreHandle_t m_mutex = nullptr;
    esp_netif_t* m_stationNetif = nullptr;
    esp_netif_t* m_accessPointNetif = nullptr;
    esp_event_handler_instance_t m_wifiHandlerInstance = nullptr;
    esp_event_handler_instance_t m_ipHandlerInstance = nullptr;
    bool m_initialized = false;
    bool m_connected = false;
    Mode m_mode = Mode::Stopped;
    uint8_t m_reconnectAttempts = 0U;
    uint8_t m_maximumReconnectAttempts = 0U;
    char m_ipAddress[16]{};
};

} // namespace control_hub::drivers::wifi
