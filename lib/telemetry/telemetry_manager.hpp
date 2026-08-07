#pragma once

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "communication/packet.hpp"
#include "communication/telemetry.hpp"

/** @file telemetry_manager.hpp @brief Thread-safe storage of BMS measurements. */

namespace control_hub::telemetry {

/**
 * @brief Decodes telemetry packets and exposes the latest coherent snapshot.
 */
class TelemetryManager final {
public:
    TelemetryManager();
    ~TelemetryManager();

    TelemetryManager(const TelemetryManager&) = delete;
    TelemetryManager& operator=(const TelemetryManager&) = delete;

    /** Decodes a received telemetry packet and replaces the current snapshot. */
    esp_err_t processPacket(const communication::Packet& packet);

    /** Copies the latest measurements and reports whether a valid packet arrived. */
    bool getLatest(communication::TelemetryData& telemetry) const;

private:
    mutable SemaphoreHandle_t m_mutex = nullptr;
    communication::TelemetryData m_latest{};
    bool m_hasTelemetry = false;
};

} // namespace control_hub::telemetry
