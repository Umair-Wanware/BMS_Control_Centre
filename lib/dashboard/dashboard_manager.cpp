#include "dashboard_manager.hpp"

#include "esp_http_server.h"
#include "esp_log.h"

#include <stdio.h>

namespace control_hub::dashboard
{

static const char *TAG = "Dashboard";

bool DashboardManager::Init()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    config.server_port = 80;

    if (httpd_start(&server, &config) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start HTTP server");
        return false;
    }

    httpd_uri_t index =
    {
        .uri = "/",
        .method = HTTP_GET,
        .handler = IndexHandler,
        .user_ctx = nullptr
    };

    httpd_register_uri_handler(server, &index);

    httpd_uri_t telemetry =
    {
        .uri = "/api/telemetry",
        .method = HTTP_GET,
        .handler = TelemetryHandler,
        .user_ctx = nullptr
    };

    httpd_register_uri_handler(server, &telemetry);

    ESP_LOGI(TAG, "Dashboard Started");

    return true;
}

void DashboardManager::Stop()
{
    if(server != nullptr)
    {
        httpd_stop(server);
        server = nullptr;
    }
}

esp_err_t DashboardManager::IndexHandler(httpd_req_t *request)
{
    static const char page[] =
R"rawliteral(
<!DOCTYPE html>
<html>

<head>

<title>BMS Control Hub</title>

<style>

body{

font-family:Arial;
background:#111;
color:white;
text-align:center;

}

.card{

width:300px;
margin:auto;
padding:20px;
background:#222;
border-radius:10px;

}

.value{

font-size:30px;

}

</style>

</head>

<body>

<h1>BMS Control Hub</h1>

<div class="card">

Voltage

<div id="voltage" class="value">--</div>

Temperature

<div id="temperature" class="value">--</div>

Current

<div id="current" class="value">--</div>

SOC

<div id="soc" class="value">--</div>

</div>

<script>

function update()
{
fetch("/api/telemetry")

.then(r=>r.json())

.then(data=>{

document.getElementById("voltage").innerHTML=data.voltage+" V";

document.getElementById("temperature").innerHTML=data.temperature+" C";

document.getElementById("current").innerHTML=data.current+" A";

document.getElementById("soc").innerHTML=data.soc+" %";

});

}

setInterval(update,500);

update();

</script>

</body>

</html>
)rawliteral";

    httpd_resp_set_type(request, "text/html");

    return httpd_resp_send(
            request,
            page,
            HTTPD_RESP_USE_STRLEN);
}

esp_err_t DashboardManager::TelemetryHandler(httpd_req_t *request)
{
    char json[128];

    float voltage = 12.45f;
    float temperature = 28.1f;
    float current = 1.52f;
    int soc = 82;

    snprintf(json,
             sizeof(json),
             "{\"voltage\":%.2f,"
             "\"temperature\":%.2f,"
             "\"current\":%.2f,"
             "\"soc\":%d}",
             voltage,
             temperature,
             current,
             soc);

    httpd_resp_set_type(request, "application/json");

    return httpd_resp_send(
            request,
            json,
            HTTPD_RESP_USE_STRLEN);
}

}