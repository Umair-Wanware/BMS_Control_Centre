#pragma once

#include "embedded_web_assets.hpp"
#include "esp_http_server.h"
#include "drivers/filesystem/filesystem_manager.hpp"
#include "telemetry/telemetry_manager.hpp"
#include "updater/firmware_updater.hpp"

namespace control_hub::dashboard
{

class DashboardManager
{
public:

    DashboardManager(telemetry::TelemetryManager* telemetryManager = nullptr,
                     updater::FirmwareUpdater* firmwareUpdater = nullptr,
                     drivers::filesystem::FileSystemManager* fileSystem = nullptr)
        : m_telemetryManager(telemetryManager),
          m_firmwareUpdater(firmwareUpdater),
          m_fileSystem(fileSystem)
    {
    }

    bool Init();

    void Stop();

private:

    static esp_err_t IndexHandler(httpd_req_t *request);
    static esp_err_t NotFoundHandler(httpd_req_t *request, httpd_err_code_t error);
    static esp_err_t HealthHandler(httpd_req_t *request);
    static esp_err_t TelemetryHandler(httpd_req_t *request);
    static esp_err_t FirmwareStatusHandler(httpd_req_t *request);
    static esp_err_t UploadFirmwareHandler(httpd_req_t *request);
    static esp_err_t StartFirmwareUpdateHandler(httpd_req_t *request);
    static esp_err_t CorsPreflightHandler(httpd_req_t *request);

    static esp_err_t serveStaticFile(httpd_req_t* request,
                                     drivers::filesystem::FileSystemManager* fileSystem,
                                     const char* relativePath);

    static esp_err_t serveEmbeddedAsset(httpd_req_t* request, const EmbeddedAsset* asset);

    httpd_handle_t server = nullptr;
    telemetry::TelemetryManager* m_telemetryManager = nullptr;
    updater::FirmwareUpdater* m_firmwareUpdater = nullptr;
    drivers::filesystem::FileSystemManager* m_fileSystem = nullptr;
};

}
