#pragma once

#include <stddef.h>
#include <stdint.h>

#include "communication/packet.hpp"

/** @file telemetry.hpp @brief Wire encoding for BMS telemetry packets. */

namespace control_hub::communication {

/**
 * @brief Latest measurements reported by the STM32 BMS.
 *
 * Fixed-point units avoid precision loss and ABI differences across the wire.
 */
struct TelemetryData {
    uint32_t voltageMillivolts = 0U;
    int32_t currentMilliamps = 0;
    int16_t temperatureCentiDegreesCelsius = 0;
    uint16_t stateOfChargeTenthsPercent = 0U;
    uint32_t faultFlags = 0U;
};

/** @brief Exact payload size for a telemetry packet. */
constexpr size_t kTelemetryPayloadLength = 16U;

/**
 * @brief Converts between the packed telemetry payload and TelemetryData.
 */
class TelemetryCodec final {
public:
    /** Decodes a valid telemetry packet into application-facing units. */
    static protocol::ValidationError decode(const Packet& packet,
                                            TelemetryData& telemetry) noexcept;

    /** Encodes telemetry into a packet payload for protocol testing or relays. */
    static protocol::ValidationError encode(const TelemetryData& telemetry,
                                            uint8_t* payload,
                                            size_t payloadCapacity) noexcept;
};

} // namespace control_hub::communication
