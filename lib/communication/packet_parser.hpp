#pragma once

#include <stddef.h>
#include <stdint.h>

#include "communication/packet.hpp"

/** @file packet_parser.hpp @brief Validation and decoding of inbound protocol packets. */

namespace control_hub::communication {

/**
 * @brief Parses serialized SPI bytes into a verified packet.
 */
class PacketParser final {
public:
    /**
     * @brief Validates and decodes the first packet in a received byte buffer.
     *
     * Extra bytes after a valid frame are tolerated. This permits a fixed-size
     * SPI receive transaction while retaining a variable-length packet format.
     */
    protocol::ValidationError parse(const uint8_t* buffer, size_t bufferLength,
                                    Packet& packet) const noexcept;
};

} // namespace control_hub::communication
