#pragma once

#include "esp_http_server.h"

namespace control_hub::dashboard
{

class DashboardManager
{
public:

    DashboardManager() = default;

    bool Init();

    void Stop();

private:

    static esp_err_t IndexHandler(httpd_req_t *request);

    static esp_err_t TelemetryHandler(httpd_req_t *request);

    httpd_handle_t server = nullptr;
};

}