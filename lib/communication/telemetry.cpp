#include "communication/telemetry.hpp"

namespace control_hub::communication {
namespace {

uint16_t readUint16Le(const uint8_t* source) noexcept
{
    return static_cast<uint16_t>(source[0]) |
           static_cast<uint16_t>(source[1]) << 8U;
}

uint32_t readUint32Le(const uint8_t* source) noexcept
{
    return static_cast<uint32_t>(source[0]) |
           static_cast<uint32_t>(source[1]) << 8U |
           static_cast<uint32_t>(source[2]) << 16U |
           static_cast<uint32_t>(source[3]) << 24U;
}

void writeUint16Le(uint8_t* destination, const uint16_t value) noexcept
{
    destination[0] = static_cast<uint8_t>(value & 0xFFU);
    destination[1] = static_cast<uint8_t>((value >> 8U) & 0xFFU);
}

void writeUint32Le(uint8_t* destination, const uint32_t value) noexcept
{
    destination[0] = static_cast<uint8_t>(value & 0xFFU);
    destination[1] = static_cast<uint8_t>((value >> 8U) & 0xFFU);
    destination[2] = static_cast<uint8_t>((value >> 16U) & 0xFFU);
    destination[3] = static_cast<uint8_t>((value >> 24U) & 0xFFU);
}

} // namespace

protocol::ValidationError TelemetryCodec::decode(const Packet& packet,
                                                 TelemetryData& telemetry) noexcept
{
    if (packet.command != Command::Telemetry) {
        return protocol::ValidationError::InvalidCommand;
    }

    if (packet.length != kTelemetryPayloadLength) {
        return protocol::ValidationError::InvalidPacketLength;
    }

    telemetry.voltageMillivolts = readUint32Le(&packet.payload[0]);
    telemetry.currentMilliamps = static_cast<int32_t>(readUint32Le(&packet.payload[4]));
    telemetry.temperatureCentiDegreesCelsius =
        static_cast<int16_t>(readUint16Le(&packet.payload[8]));
    telemetry.stateOfChargeTenthsPercent = readUint16Le(&packet.payload[10]);
    telemetry.faultFlags = readUint32Le(&packet.payload[12]);
    return protocol::ValidationError::None;
}

protocol::ValidationError TelemetryCodec::encode(const TelemetryData& telemetry,
                                                 uint8_t* payload,
                                                 const size_t payloadCapacity) noexcept
{
    if (payload == nullptr || payloadCapacity < kTelemetryPayloadLength) {
        return protocol::ValidationError::InsufficientData;
    }

    writeUint32Le(&payload[0], telemetry.voltageMillivolts);
    writeUint32Le(&payload[4], static_cast<uint32_t>(telemetry.currentMilliamps));
    writeUint16Le(&payload[8],
                  static_cast<uint16_t>(telemetry.temperatureCentiDegreesCelsius));
    writeUint16Le(&payload[10], telemetry.stateOfChargeTenthsPercent);
    writeUint32Le(&payload[12], telemetry.faultFlags);
    return protocol::ValidationError::None;
}

} // namespace control_hub::communication
