#pragma once

#include <stddef.h>
#include <stdint.h>

#include "communication/packet.hpp"

/** @file packet_builder.hpp @brief Safe serialization of outbound protocol packets. */

namespace control_hub::communication {

/**
 * @brief Builds and serializes CRC-protected packets for SPI transport.
 */
class PacketBuilder final {
public:
    /** Creates a packet and calculates its packet CRC16. */
    protocol::ValidationError build(Command command, uint16_t sequence,
                                    const uint8_t* payload, size_t payloadLength,
                                    Packet& packet) const noexcept;

    /** Serializes a valid packet using little-endian multibyte fields. */
    protocol::ValidationError serialize(const Packet& packet, uint8_t* buffer,
                                        size_t bufferCapacity,
                                        size_t& bytesWritten) const noexcept;

    /** Calculates the packet CRC16 from the packet's protected fields. */
    uint16_t calculateCrc(const Packet& packet) const noexcept;
};

} // namespace control_hub::communication
