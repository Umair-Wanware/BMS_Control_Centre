#pragma once

#include <stddef.h>
#include <stdint.h>

/** @file crc32.hpp @brief IEEE CRC-32 implementation for firmware images. */

namespace control_hub::communication {

/**
 * @brief Calculates the IEEE 802.3 CRC-32 used to verify firmware images.
 */
class Crc32 final {
public:
    static constexpr uint32_t kInitialValue = 0xFFFFFFFFUL;

    /** Returns the initial state for incremental image checksum calculation. */
    static constexpr uint32_t initialize() noexcept { return kInitialValue; }

    /** Updates an in-progress CRC with a byte range. */
    static uint32_t update(uint32_t crc, const uint8_t* data, size_t length) noexcept;

    /** Applies the final XOR to an incremental CRC state. */
    static constexpr uint32_t finalize(const uint32_t crc) noexcept
    {
        return crc ^ 0xFFFFFFFFUL;
    }

    /** Calculates a CRC over one complete byte range. */
    static uint32_t calculate(const uint8_t* data, size_t length) noexcept;
};

} // namespace control_hub::communication
