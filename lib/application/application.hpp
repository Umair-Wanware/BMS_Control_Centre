#pragma once

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "communication/communication_manager.hpp"
#include "dashboard/dashboard_manager.hpp"
#include "drivers/filesystem/filesystem_manager.hpp"
#include "drivers/spi/spi_master.hpp"
#include "drivers/wifi/wifi_manager.hpp"
#include "storage/storage_manager.hpp"
#include "telemetry/telemetry_manager.hpp"
#include "updater/firmware_updater.hpp"
#include "utils/logger.hpp"

/** @file application.hpp @brief Top-level composition root for the Control Hub. */

namespace control_hub::application {

/**
 * @brief Initializes hardware and owns the long-lived Control Hub services.
 */
class Application final {
public:
    Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    /** Initializes persistent storage, connectivity, SPI, protocol services, and tasks. */
    esp_err_t initialize();

    /** Keeps the application alive after successful initialization. */
    void run() const;

private:
    static void telemetryTaskEntry(void* argument);
    void telemetryTask();
    esp_err_t initializeNvs() const;
    esp_err_t startNetwork();

    utils::Logger m_logger{"Application"};
    drivers::filesystem::FileSystemManager m_fileSystem;
    storage::StorageManager m_storage;
    drivers::wifi::WiFiManager m_wifi;
    drivers::spi::SpiMaster m_spi;
    communication::CommunicationManager m_communication;
    telemetry::TelemetryManager m_telemetry;
    updater::FirmwareUpdater m_firmwareUpdater;
    dashboard::DashboardManager m_dashboard;
    TaskHandle_t m_telemetryTask = nullptr;
    bool m_initialized = false;
};

} // namespace control_hub::application
