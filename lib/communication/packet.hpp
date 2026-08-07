#pragma once

#include <stddef.h>
#include <stdint.h>

#include "communication/commands.hpp"
#include "communication/protocol.hpp"

/**
 * @file packet.hpp
 * @brief In-memory representation and structural validation of BMS packets.
 */

namespace control_hub::communication {

/**
 * @brief A decoded BMS protocol packet.
 *
 * Only the first @ref length bytes in @ref payload belong to the packet. The
 * CRC16 protects the serialized header, command, sequence, length, and payload.
 */
struct Packet {
    uint16_t header = protocol::makeHeader();
    Command command = Command::None;
    uint16_t sequence = 0U;
    uint16_t length = 0U;
    uint8_t payload[protocol::kMaximumPayloadLength]{};
    uint16_t crc16 = 0U;
};

/**
 * @brief Returns whether a packet's non-CRC fields can be represented safely.
 *
 * CRC verification is intentionally performed by PacketParser because it
 * operates on the exact serialized byte sequence received from SPI.
 */
constexpr protocol::ValidationError validatePacketStructure(const Packet& packet) noexcept
{
    if (!protocol::hasValidHeaderMagic(packet.header)) {
        return protocol::ValidationError::InvalidHeader;
    }

    if (!protocol::isSupportedVersion(protocol::getVersion(packet.header))) {
        return protocol::ValidationError::UnsupportedVersion;
    }

    if (!isValidCommand(static_cast<uint8_t>(packet.command))) {
        return protocol::ValidationError::InvalidCommand;
    }

    if (!protocol::isValidPayloadLength(packet.length)) {
        return protocol::ValidationError::PayloadTooLarge;
    }

    return protocol::ValidationError::None;
}

/**
 * @brief Returns the number of bytes used when this packet is serialized.
 */
constexpr size_t serializedPacketLength(const Packet& packet) noexcept
{
    return protocol::serializedPacketLength(packet.length);
}

} // namespace control_hub::communication
