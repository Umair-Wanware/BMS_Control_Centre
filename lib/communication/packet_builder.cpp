#include "communication/packet_builder.hpp"

#include <string.h>

#include "communication/crc16.hpp"

namespace control_hub::communication {
namespace {

void writeUint16Le(uint8_t* destination, const uint16_t value) noexcept
{
    destination[0] = static_cast<uint8_t>(value & 0x00FFU);
    destination[1] = static_cast<uint8_t>((value >> 8U) & 0x00FFU);
}

} // namespace

protocol::ValidationError PacketBuilder::build(const Command command, const uint16_t sequence,
                                               const uint8_t* payload,
                                               const size_t payloadLength,
                                               Packet& packet) const noexcept
{
    if (!isValidCommand(static_cast<uint8_t>(command))) {
        return protocol::ValidationError::InvalidCommand;
    }

    if (!protocol::isValidPayloadLength(payloadLength)) {
        return protocol::ValidationError::PayloadTooLarge;
    }

    if (payload == nullptr && payloadLength != 0U) {
        return protocol::ValidationError::InsufficientData;
    }

    packet = Packet{};
    packet.command = command;
    packet.sequence = sequence;
    packet.length = static_cast<uint16_t>(payloadLength);

    if (payloadLength != 0U) {
        memcpy(packet.payload, payload, payloadLength);
    }

    packet.crc16 = calculateCrc(packet);
    return protocol::ValidationError::None;
}

protocol::ValidationError PacketBuilder::serialize(const Packet& packet, uint8_t* buffer,
                                                   const size_t bufferCapacity,
                                                   size_t& bytesWritten) const noexcept
{
    bytesWritten = 0U;

    const protocol::ValidationError validation = validatePacketStructure(packet);
    if (validation != protocol::ValidationError::None) {
        return validation;
    }

    const size_t packetLength = serializedPacketLength(packet);
    if (buffer == nullptr || bufferCapacity < packetLength) {
        return protocol::ValidationError::InsufficientData;
    }

    writeUint16Le(&buffer[0], packet.header);
    buffer[2] = static_cast<uint8_t>(packet.command);
    writeUint16Le(&buffer[3], packet.sequence);
    writeUint16Le(&buffer[5], packet.length);

    if (packet.length != 0U) {
        memcpy(&buffer[protocol::kPacketPrefixLength], packet.payload, packet.length);
    }

    writeUint16Le(&buffer[packetLength - protocol::kPacketCrcLength], calculateCrc(packet));
    bytesWritten = packetLength;
    return protocol::ValidationError::None;
}

uint16_t PacketBuilder::calculateCrc(const Packet& packet) const noexcept
{
    const protocol::ValidationError validation = validatePacketStructure(packet);
    if (validation != protocol::ValidationError::None) {
        return 0U;
    }

    uint8_t protectedBytes[protocol::kMaximumPacketLength - protocol::kPacketCrcLength]{};
    writeUint16Le(&protectedBytes[0], packet.header);
    protectedBytes[2] = static_cast<uint8_t>(packet.command);
    writeUint16Le(&protectedBytes[3], packet.sequence);
    writeUint16Le(&protectedBytes[5], packet.length);

    if (packet.length != 0U) {
        memcpy(&protectedBytes[protocol::kPacketPrefixLength], packet.payload, packet.length);
    }

    return Crc16::calculate(protectedBytes,
                            protocol::kPacketPrefixLength + packet.length);
}

} // namespace control_hub::communication
