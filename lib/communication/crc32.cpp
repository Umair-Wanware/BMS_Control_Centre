#include "communication/crc32.hpp"

namespace control_hub::communication {

uint32_t Crc32::update(uint32_t crc, const uint8_t* data, const size_t length) noexcept
{
    if (data == nullptr && length != 0U) {
        return 0U;
    }

    for (size_t index = 0U; index < length; ++index) {
        crc ^= data[index];
        for (uint8_t bit = 0U; bit < 8U; ++bit) {
            crc = (crc & 1U) != 0U ? (crc >> 1U) ^ 0xEDB88320UL : crc >> 1U;
        }
    }

    return crc;
}

uint32_t Crc32::calculate(const uint8_t* data, const size_t length) noexcept
{
    return finalize(update(initialize(), data, length));
}

} // namespace control_hub::communication
