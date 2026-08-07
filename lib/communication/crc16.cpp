#include "communication/crc16.hpp"

namespace control_hub::communication {

uint16_t Crc16::update(uint16_t crc, const uint8_t* data, const size_t length) noexcept
{
    if (data == nullptr && length != 0U) {
        return 0U;
    }

    for (size_t index = 0U; index < length; ++index) {
        crc ^= static_cast<uint16_t>(data[index]) << 8U;
        for (uint8_t bit = 0U; bit < 8U; ++bit) {
            crc = (crc & 0x8000U) != 0U
                      ? static_cast<uint16_t>((crc << 1U) ^ 0x1021U)
                      : static_cast<uint16_t>(crc << 1U);
        }
    }

    return crc;
}

uint16_t Crc16::calculate(const uint8_t* data, const size_t length) noexcept
{
    return update(kInitialValue, data, length);
}

} // namespace control_hub::communication
