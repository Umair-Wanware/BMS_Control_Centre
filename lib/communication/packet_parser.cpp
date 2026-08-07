#include "communication/packet_parser.hpp"

#include <string.h>

#include "communication/crc16.hpp"

namespace control_hub::communication {
namespace {

uint16_t readUint16Le(const uint8_t* source) noexcept
{
    return static_cast<uint16_t>(source[0]) |
           static_cast<uint16_t>(source[1]) << 8U;
}

} // namespace

protocol::ValidationError PacketParser::parse(const uint8_t* buffer, const size_t bufferLength,
                                               Packet& packet) const noexcept
{
    if (buffer == nullptr || bufferLength < protocol::kMinimumPacketLength) {
        return protocol::ValidationError::InsufficientData;
    }

    Packet decoded{};
    decoded.header = readUint16Le(&buffer[0]);
    if (!protocol::hasValidHeaderMagic(decoded.header)) {
        return protocol::ValidationError::InvalidHeader;
    }

    if (!protocol::isSupportedVersion(protocol::getVersion(decoded.header))) {
        return protocol::ValidationError::UnsupportedVersion;
    }

    const uint8_t commandValue = buffer[2];
    if (!isValidCommand(commandValue)) {
        return protocol::ValidationError::InvalidCommand;
    }
    decoded.command = static_cast<Command>(commandValue);

    decoded.sequence = readUint16Le(&buffer[3]);
    decoded.length = readUint16Le(&buffer[5]);
    if (!protocol::isValidPayloadLength(decoded.length)) {
        return protocol::ValidationError::PayloadTooLarge;
    }

    const size_t expectedLength = serializedPacketLength(decoded);
    if (bufferLength < expectedLength) {
        return protocol::ValidationError::InvalidPacketLength;
    }

    const uint16_t receivedCrc =
        readUint16Le(&buffer[expectedLength - protocol::kPacketCrcLength]);
    const uint16_t calculatedCrc =
        Crc16::calculate(buffer, expectedLength - protocol::kPacketCrcLength);
    if (receivedCrc != calculatedCrc) {
        return protocol::ValidationError::CrcMismatch;
    }

    if (decoded.length != 0U) {
        memcpy(decoded.payload, &buffer[protocol::kPacketPrefixLength], decoded.length);
    }
    decoded.crc16 = receivedCrc;
    packet = decoded;
    return protocol::ValidationError::None;
}

} // namespace control_hub::communication
