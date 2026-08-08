#pragma once

#include "esp_http_server.h"
#include "telemetry/telemetry_manager.hpp"

namespace control_hub::dashboard
{

class DashboardManager
{
public:

    explicit DashboardManager(telemetry::TelemetryManager* telemetryManager = nullptr)
        : m_telemetryManager(telemetryManager)
    {
    }

    bool Init();

    void Stop();

private:

    static esp_err_t IndexHandler(httpd_req_t *request);

    static esp_err_t TelemetryHandler(httpd_req_t *request);

    httpd_handle_t server = nullptr;
    telemetry::TelemetryManager* m_telemetryManager = nullptr;
};

}