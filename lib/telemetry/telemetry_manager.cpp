#include "telemetry/telemetry_manager.hpp"

namespace control_hub::telemetry {

TelemetryManager::TelemetryManager()
{
    m_mutex = xSemaphoreCreateMutex();
}

TelemetryManager::~TelemetryManager()
{
    if (m_mutex != nullptr) {
        vSemaphoreDelete(m_mutex);
    }
}

esp_err_t TelemetryManager::processPacket(const communication::Packet& packet)
{
    communication::TelemetryData decoded{};
    const communication::protocol::ValidationError result =
        communication::TelemetryCodec::decode(packet, decoded);
    if (result != communication::protocol::ValidationError::None) {
        return result == communication::protocol::ValidationError::InvalidCommand
                   ? ESP_ERR_INVALID_ARG
                   : ESP_ERR_INVALID_SIZE;
    }

    if (m_mutex == nullptr || xSemaphoreTake(m_mutex, pdMS_TO_TICKS(1000U)) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }

    m_latest = decoded;
    m_hasTelemetry = true;
    xSemaphoreGive(m_mutex);
    return ESP_OK;
}

bool TelemetryManager::getLatest(communication::TelemetryData& telemetry) const
{
    if (m_mutex == nullptr || xSemaphoreTake(m_mutex, pdMS_TO_TICKS(1000U)) != pdTRUE) {
        return false;
    }

    telemetry = m_latest;
    const bool hasTelemetry = m_hasTelemetry;
    xSemaphoreGive(m_mutex);
    return hasTelemetry;
}

} // namespace control_hub::telemetry
