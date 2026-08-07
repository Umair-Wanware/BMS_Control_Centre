#pragma once

#include <stddef.h>
#include <stdint.h>

#include <vector>

#include "driver/spi_master.h"
#include "esp_err.h"

/** @file spi_master.hpp @brief ESP-IDF SPI master driver adapter. */

namespace control_hub::drivers::spi {

/**
 * @brief Owns one ESP-IDF SPI bus and one device attached to it.
 *
 * This class deliberately has no BMS packet knowledge. It transfers opaque
 * byte buffers, leaving framing, sequencing, and checksums to communication.
 */
class SpiMaster final {
public:
    /** @brief Electrical and timing configuration for the SPI device. */
    struct Config {
        spi_host_device_t host = SPI2_HOST;
        int mosiPin = 7;
        int misoPin = 2;
        int clockPin = 6;
        int chipSelectPin = 10;
        int clockSpeedHz = 1000000;
        uint8_t mode = 0U;
        size_t maximumTransferSize = 137U;
    };

    SpiMaster();
    explicit SpiMaster(Config config);
    ~SpiMaster();

    SpiMaster(const SpiMaster&) = delete;
    SpiMaster& operator=(const SpiMaster&) = delete;

    /** Allocates the SPI bus and registers the configured device. */
    esp_err_t initialize();

    /** Releases the registered device and SPI bus. Safe to call repeatedly. */
    esp_err_t deinitialize();

    /** Performs one full-duplex SPI transaction. */
    esp_err_t transfer(const uint8_t* transmit, uint8_t* receive, size_t length);

    /** Sends bytes while discarding the simultaneously received bytes. */
    esp_err_t send(const uint8_t* transmit, size_t length);

    /** Clocks in bytes by transmitting zero-valued dummy bytes. */
    esp_err_t receive(uint8_t* receive, size_t length);

    /** Returns whether the driver currently owns an initialized device. */
    bool isInitialized() const noexcept;

private:
    Config m_config;
    spi_device_handle_t m_device = nullptr;
    bool m_busInitialized = false;
    std::vector<uint8_t> m_dummyTransmit;
};

} // namespace control_hub::drivers::spi
