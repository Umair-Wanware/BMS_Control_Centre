#pragma once

#include <stddef.h>
#include <stdint.h>

/** @file crc16.hpp @brief CRC-16/CCITT-FALSE packet checksum implementation. */

namespace control_hub::communication {

/**
 * @brief Calculates the CRC-16/CCITT-FALSE checksum used by protocol packets.
 *
 * Parameters: polynomial 0x1021, initial value 0xFFFF, no reflection, and no
 * final XOR. These settings must match the STM32 bootloader implementation.
 */
class Crc16 final {
public:
    static constexpr uint16_t kInitialValue = 0xFFFFU;

    /** Updates an in-progress CRC with a byte range. */
    static uint16_t update(uint16_t crc, const uint8_t* data, size_t length) noexcept;

    /** Calculates a CRC over one complete byte range. */
    static uint16_t calculate(const uint8_t* data, size_t length) noexcept;
};

} // namespace control_hub::communication
