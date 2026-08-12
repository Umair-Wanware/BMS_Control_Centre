#include "dashboard_manager.hpp"

#include "communication/telemetry.hpp"
#include "esp_http_server.h"
#include "esp_log.h"

#include <algorithm>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <vector>

namespace {

const char* TAG = "Dashboard";

constexpr size_t kStaticFileChunkSize = 4096U;

const char* firmwareStateToString(const control_hub::updater::FirmwareUpdateState state) noexcept
{
    switch (state) {
        case control_hub::updater::FirmwareUpdateState::Idle:
            return "Idle";
        case control_hub::updater::FirmwareUpdateState::CalculatingChecksum:
            return "CalculatingChecksum";
        case control_hub::updater::FirmwareUpdateState::Starting:
            return "Starting";
        case control_hub::updater::FirmwareUpdateState::Transferring:
            return "Transferring";
        case control_hub::updater::FirmwareUpdateState::Verifying:
            return "Verifying";
        case control_hub::updater::FirmwareUpdateState::Complete:
            return "Complete";
        case control_hub::updater::FirmwareUpdateState::Failed:
            return "Failed";
        case control_hub::updater::FirmwareUpdateState::Cancelled:
            return "Cancelled";
        default:
            return "Unknown";
    }
}

const char* contentTypeForPath(const char* path) noexcept
{
    const char* extension = strrchr(path, '.');
    if (extension == nullptr) {
        return "application/octet-stream";
    }

    if (strcmp(extension, ".html") == 0) {
        return "text/html";
    }
    if (strcmp(extension, ".css") == 0) {
        return "text/css";
    }
    if (strcmp(extension, ".js") == 0) {
        return "application/javascript";
    }
    if (strcmp(extension, ".json") == 0) {
        return "application/json";
    }
    if (strcmp(extension, ".svg") == 0) {
        return "image/svg+xml";
    }
    if (strcmp(extension, ".png") == 0) {
        return "image/png";
    }
    if (strcmp(extension, ".ico") == 0) {
        return "image/x-icon";
    }

    return "application/octet-stream";
}

void setApiCorsHeaders(httpd_req_t* request)
{
    httpd_resp_set_hdr(request, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(request, "Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    httpd_resp_set_hdr(request, "Access-Control-Allow-Headers", "Content-Type");
}

control_hub::dashboard::DashboardManager* managerFromRequest(httpd_req_t* request)
{
    if (request == nullptr || request->handle == nullptr) {
        return nullptr;
    }

    return static_cast<control_hub::dashboard::DashboardManager*>(
        httpd_get_global_user_ctx(request->handle));
}

} // namespace

namespace control_hub::dashboard
{

bool DashboardManager::Init()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;
    config.max_uri_handlers = 12U;
    config.stack_size = 10240U;
    config.recv_wait_timeout = 60U;
    config.lru_purge_enable = true;
    config.max_open_sockets = 7U;
    config.global_user_ctx = this;

    if (httpd_start(&server, &config) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start HTTP server");
        return false;
    }

    if (httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, NotFoundHandler) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register 404 handler");
        return false;
    }

    const httpd_uri_t routes[] = {
        {"/api/health", HTTP_GET, HealthHandler, this},
        {"/api/telemetry", HTTP_GET, TelemetryHandler, this},
        {"/api/firmware", HTTP_GET, FirmwareStatusHandler, this},
        {"/api/firmware/upload", HTTP_POST, UploadFirmwareHandler, this},
        {"/api/firmware/upload", HTTP_OPTIONS, CorsPreflightHandler, this},
        {"/api/firmware/start", HTTP_POST, StartFirmwareUpdateHandler, this},
        {"/api/firmware/start", HTTP_OPTIONS, CorsPreflightHandler, this},
        {"/", HTTP_GET, IndexHandler, this},
    };

    for (const httpd_uri_t& route : routes) {
        if (httpd_register_uri_handler(server, &route) != ESP_OK) {
            ESP_LOGE(TAG, "Failed to register handler for %s", route.uri);
            return false;
        }
    }

    ESP_LOGI(TAG, "HTTP server ready on port 80");
    ESP_LOGI(TAG, "Dashboard: http://192.168.4.1/  (AP mode) or device IP (STA mode)");
    ESP_LOGI(TAG, "Health check: http://192.168.4.1/api/health");
    return true;
}

esp_err_t DashboardManager::CorsPreflightHandler(httpd_req_t *request)
{
    setApiCorsHeaders(request);
    httpd_resp_set_hdr(request, "Access-Control-Max-Age", "3600");
    return httpd_resp_send(request, nullptr, 0);
}

void DashboardManager::Stop()
{
    if (server != nullptr) {
        httpd_stop(server);
        server = nullptr;
    }
}

esp_err_t DashboardManager::serveEmbeddedAsset(httpd_req_t* request, const EmbeddedAsset* asset)
{
    if (asset == nullptr) {
        httpd_resp_set_status(request, "404 Not Found");
        return httpd_resp_sendstr(request, "Not found");
    }

    httpd_resp_set_type(request, asset->contentType);

    size_t offset = 0U;
    while (offset < asset->size) {
        const size_t chunkSize = std::min(kStaticFileChunkSize, asset->size - offset);
        const esp_err_t sendResult =
            httpd_resp_send_chunk(request, asset->content + offset, chunkSize);
        if (sendResult != ESP_OK) {
            return sendResult;
        }
        offset += chunkSize;
    }

    return httpd_resp_send_chunk(request, nullptr, 0);
}

esp_err_t DashboardManager::serveStaticFile(httpd_req_t* request,
                                            drivers::filesystem::FileSystemManager* fileSystem,
                                            const char* relativePath)
{
    if (relativePath == nullptr || relativePath[0] != '/') {
        httpd_resp_set_status(request, "404 Not Found");
        return httpd_resp_sendstr(request, "Not found");
    }

    const EmbeddedAsset* embeddedAsset = findEmbeddedAsset(relativePath);
    if (embeddedAsset != nullptr) {
        return serveEmbeddedAsset(request, embeddedAsset);
    }

    if (fileSystem != nullptr && fileSystem->isMounted()) {
        size_t fileSize = 0U;
        if (fileSystem->getFileSize(relativePath, fileSize) == ESP_OK && fileSize > 0U) {
            httpd_resp_set_type(request, contentTypeForPath(relativePath));

            std::vector<uint8_t> buffer(kStaticFileChunkSize);
            size_t offset = 0U;
            while (offset < fileSize) {
                size_t bytesRead = 0U;
                const size_t requested = std::min(buffer.size(), fileSize - offset);
                const esp_err_t readResult = fileSystem->readFileChunk(
                    relativePath, offset, buffer.data(), requested, bytesRead);
                if (readResult != ESP_OK || bytesRead == 0U) {
                    httpd_resp_set_status(request, "500 Internal Server Error");
                    return httpd_resp_sendstr(request, "Read failed");
                }

                const esp_err_t sendResult = httpd_resp_send_chunk(
                    request, reinterpret_cast<const char*>(buffer.data()), bytesRead);
                if (sendResult != ESP_OK) {
                    return sendResult;
                }

                offset += bytesRead;
            }

            return httpd_resp_send_chunk(request, nullptr, 0);
        }
    }

    httpd_resp_set_status(request, "404 Not Found");
    return httpd_resp_sendstr(request, "Not found");
}

esp_err_t DashboardManager::IndexHandler(httpd_req_t *request)
{
    DashboardManager* manager = static_cast<DashboardManager*>(request->user_ctx);
    if (manager == nullptr) {
        httpd_resp_set_status(request, "500 Internal Server Error");
        return httpd_resp_sendstr(request, "Server unavailable");
    }

    return serveStaticFile(request, manager->m_fileSystem, "/index.html");
}

esp_err_t DashboardManager::NotFoundHandler(httpd_req_t *request, httpd_err_code_t error)
{
    if (error != HTTPD_404_NOT_FOUND) {
        return ESP_FAIL;
    }

    DashboardManager* manager = managerFromRequest(request);
    if (manager == nullptr || request->uri[0] == '\0') {
        return httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "Not found");
    }

    if (strncmp(request->uri, "/api/", 5) == 0) {
        return httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "Not found");
    }

    const esp_err_t result = serveStaticFile(request, manager->m_fileSystem, request->uri);
    if (result == ESP_OK) {
        return ESP_OK;
    }

    return httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "Not found");
}

esp_err_t DashboardManager::HealthHandler(httpd_req_t *request)
{
    DashboardManager* manager = static_cast<DashboardManager*>(request->user_ctx);
    char json[160];
    const bool filesystemReady =
        manager != nullptr && manager->m_fileSystem != nullptr && manager->m_fileSystem->isMounted();
    const bool telemetryReady = manager != nullptr && manager->m_telemetryManager != nullptr;
    const bool updaterReady = manager != nullptr && manager->m_firmwareUpdater != nullptr;
    bool telemetryValid = false;

    if (telemetryReady) {
        communication::TelemetryData data{};
        telemetryValid = manager->m_telemetryManager->getLatest(data);
    }

    snprintf(json,
             sizeof(json),
             "{\"status\":\"ok\",\"filesystem\":%s,\"telemetry_ready\":%s,"
             "\"telemetry_valid\":%s,\"updater_ready\":%s,\"embedded_ui\":true}",
             filesystemReady ? "true" : "false",
             telemetryReady ? "true" : "false",
             telemetryValid ? "true" : "false",
             updaterReady ? "true" : "false");

    httpd_resp_set_type(request, "application/json");
    setApiCorsHeaders(request);
    return httpd_resp_send(request, json, HTTPD_RESP_USE_STRLEN);
}

esp_err_t DashboardManager::TelemetryHandler(httpd_req_t *request)
{
    DashboardManager* manager = static_cast<DashboardManager*>(request->user_ctx);
    char json[256];
    bool valid = false;

    uint32_t voltage = 0U;
    int32_t current = 0;
    int16_t temperature = 0;
    uint16_t soc = 0U;
    uint32_t faultFlags = 0U;

    if (manager != nullptr && manager->m_telemetryManager != nullptr) {
        communication::TelemetryData data{};
        if (manager->m_telemetryManager->getLatest(data)) {
            valid = true;
            voltage = data.voltageMillivolts;
            current = data.currentMilliamps;
            temperature = data.temperatureCentiDegreesCelsius;
            soc = data.stateOfChargeTenthsPercent;
            faultFlags = data.faultFlags;
        }
    }

    snprintf(json,
             sizeof(json),
             "{\"valid\":%s,"
             "\"voltage_mv\":%" PRIu32 ","
             "\"current_ma\":%" PRId32 ","
             "\"temperature_centi_c\":%" PRId16 ","
             "\"soc_tenths_percent\":%" PRIu16 ","
             "\"fault_flags\":%" PRIu32 "}",
             valid ? "true" : "false",
             voltage,
             current,
             temperature,
             soc,
             faultFlags);

    httpd_resp_set_type(request, "application/json");
    setApiCorsHeaders(request);
    return httpd_resp_send(request, json, HTTPD_RESP_USE_STRLEN);
}

esp_err_t DashboardManager::FirmwareStatusHandler(httpd_req_t *request)
{
    DashboardManager* manager = static_cast<DashboardManager*>(request->user_ctx);
    char json[256];
    const char* state = "Idle";
    uint32_t progress = 0U;
    bool running = false;

    if (manager != nullptr && manager->m_firmwareUpdater != nullptr) {
        const auto snapshot = manager->m_firmwareUpdater->progress();
        state = firmwareStateToString(snapshot.state);
        progress = snapshot.imageSizeBytes == 0U ? 0U :
            (static_cast<uint32_t>((snapshot.transferredBytes * 100U) / snapshot.imageSizeBytes));
        running = manager->m_firmwareUpdater->isRunning();
    }

    snprintf(json, sizeof(json),
             "{\"state\":\"%s\",\"running\":%s,\"progress\":%" PRIu32 ",\"image_path\":\"/firmware.bin\"}",
             state, running ? "true" : "false", progress);

    httpd_resp_set_type(request, "application/json");
    setApiCorsHeaders(request);
    return httpd_resp_send(request, json, HTTPD_RESP_USE_STRLEN);
}

esp_err_t DashboardManager::UploadFirmwareHandler(httpd_req_t *request)
{
    DashboardManager* manager = static_cast<DashboardManager*>(request->user_ctx);
    if (manager == nullptr || manager->m_fileSystem == nullptr || !manager->m_fileSystem->isMounted()) {
        httpd_resp_set_type(request, "application/json");
        httpd_resp_set_status(request, "503 Service Unavailable");
        setApiCorsHeaders(request);
        return httpd_resp_sendstr(request, "{\"error\":\"filesystem unavailable\"}");
    }

    size_t contentLength = request->content_len;
    if (contentLength == 0U) {
        char contentLengthHeader[32]{};
        if (httpd_req_get_hdr_value_str(request, "Content-Length", contentLengthHeader,
                                        sizeof(contentLengthHeader)) == ESP_OK) {
            contentLength = static_cast<size_t>(strtoul(contentLengthHeader, nullptr, 10));
        }
    }

    drivers::filesystem::FileSystemManager::StreamingWriteHandle writeHandle{};
    esp_err_t writeResult = manager->m_fileSystem->beginStreamingWrite("/firmware.bin", writeHandle);
    if (writeResult != ESP_OK) {
        httpd_resp_set_type(request, "application/json");
        httpd_resp_set_status(request, "500 Internal Server Error");
        setApiCorsHeaders(request);
        return httpd_resp_sendstr(request, "{\"error\":\"open failed\"}");
    }

    std::vector<uint8_t> buffer(4096U);
    size_t totalReceived = 0U;

    if (contentLength > 0U) {
        while (totalReceived < contentLength) {
            const size_t bytesToRead = std::min(buffer.size(), contentLength - totalReceived);
            const int received =
                httpd_req_recv(request, reinterpret_cast<char*>(buffer.data()), bytesToRead);
            if (received <= 0) {
                manager->m_fileSystem->abortStreamingWrite(writeHandle);
                httpd_resp_set_type(request, "application/json");
                httpd_resp_set_status(request, "400 Bad Request");
                setApiCorsHeaders(request);
                return httpd_resp_sendstr(request, "{\"error\":\"upload failed\"}");
            }

            writeResult = manager->m_fileSystem->appendStreamingWrite(
                writeHandle, buffer.data(), static_cast<size_t>(received));
            if (writeResult != ESP_OK) {
                manager->m_fileSystem->abortStreamingWrite(writeHandle);
                httpd_resp_set_type(request, "application/json");
                httpd_resp_set_status(request, "500 Internal Server Error");
                setApiCorsHeaders(request);
                return httpd_resp_sendstr(request, "{\"error\":\"write failed\"}");
            }

            totalReceived += static_cast<size_t>(received);
        }
    } else {
        while (true) {
            const int received =
                httpd_req_recv(request, reinterpret_cast<char*>(buffer.data()), buffer.size());
            if (received == HTTPD_SOCK_ERR_TIMEOUT) {
                continue;
            }
            if (received <= 0) {
                break;
            }

            writeResult = manager->m_fileSystem->appendStreamingWrite(
                writeHandle, buffer.data(), static_cast<size_t>(received));
            if (writeResult != ESP_OK) {
                manager->m_fileSystem->abortStreamingWrite(writeHandle);
                httpd_resp_set_type(request, "application/json");
                httpd_resp_set_status(request, "500 Internal Server Error");
                setApiCorsHeaders(request);
                return httpd_resp_sendstr(request, "{\"error\":\"write failed\"}");
            }

            totalReceived += static_cast<size_t>(received);
        }
    }

    if (totalReceived == 0U) {
        manager->m_fileSystem->abortStreamingWrite(writeHandle);
        httpd_resp_set_type(request, "application/json");
        httpd_resp_set_status(request, "400 Bad Request");
        setApiCorsHeaders(request);
        return httpd_resp_sendstr(request, "{\"error\":\"no content received\"}");
    }

    writeResult = manager->m_fileSystem->finishStreamingWrite(writeHandle);
    if (writeResult != ESP_OK) {
        manager->m_fileSystem->abortStreamingWrite(writeHandle);
        httpd_resp_set_type(request, "application/json");
        httpd_resp_set_status(request, "500 Internal Server Error");
        setApiCorsHeaders(request);
        return httpd_resp_sendstr(request, "{\"error\":\"save failed\"}");
    }

    ESP_LOGI(TAG, "Saved firmware upload (%u bytes)", totalReceived);
    httpd_resp_set_type(request, "application/json");
    setApiCorsHeaders(request);
    return httpd_resp_sendstr(request, "{\"status\":\"uploaded\",\"path\":\"/firmware.bin\"}");
}

esp_err_t DashboardManager::StartFirmwareUpdateHandler(httpd_req_t *request)
{
    DashboardManager* manager = static_cast<DashboardManager*>(request->user_ctx);
    if (manager == nullptr || manager->m_firmwareUpdater == nullptr) {
        httpd_resp_set_type(request, "application/json");
        httpd_resp_set_status(request, "400 Bad Request");
        setApiCorsHeaders(request);
        return httpd_resp_sendstr(request, "{\"error\":\"updater unavailable\"}");
    }

    constexpr size_t kBufferSize = 128U;
    char buffer[kBufferSize]{};
    const int received = httpd_req_recv(request, buffer,
                                        std::min(static_cast<size_t>(request->content_len),
                                                 sizeof(buffer) - 1U));
    std::string body;
    if (received > 0) {
        buffer[received] = '\0';
        body = buffer;
    }

    std::string imagePath = "/firmware.bin";
    const char* pathToken = strstr(body.c_str(), "\"path\"");
    if (pathToken != nullptr) {
        const char* start = strchr(pathToken, ':');
        if (start != nullptr) {
            const char* value = start + 1;
            while (*value == ' ' || *value == '\t' || *value == '"') {
                ++value;
            }
            const char* end = value;
            while (*end != '\0' && *end != '"' && *end != ',' && *end != '}') {
                ++end;
            }
            if (end > value) {
                imagePath.assign(value, static_cast<size_t>(end - value));
            }
        }
    }

    const esp_err_t result = manager->m_firmwareUpdater->startUpdate(imagePath.c_str());
    if (result != ESP_OK) {
        httpd_resp_set_type(request, "application/json");
        httpd_resp_set_status(request, "500 Internal Server Error");
        setApiCorsHeaders(request);
        return httpd_resp_sendstr(request, "{\"error\":\"start failed\"}");
    }

    httpd_resp_set_type(request, "application/json");
    setApiCorsHeaders(request);
    return httpd_resp_sendstr(request, "{\"status\":\"started\",\"path\":\"/firmware.bin\"}");
}

}
