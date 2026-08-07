#include "drivers/spi/spi_master.hpp"

#include <limits.h>

namespace control_hub::drivers::spi {

SpiMaster::SpiMaster() : SpiMaster(Config{}) {}

SpiMaster::SpiMaster(Config config) : m_config(config) {}

SpiMaster::~SpiMaster()
{
    deinitialize();
}

esp_err_t SpiMaster::initialize()
{
    if (isInitialized()) {
        return ESP_OK;
    }

    if (m_config.maximumTransferSize == 0U ||
        m_config.maximumTransferSize > static_cast<size_t>(INT_MAX) / 8U ||
        m_config.clockSpeedHz <= 0 || m_config.mode > 3U) {
        return ESP_ERR_INVALID_ARG;
    }

    spi_bus_config_t busConfig{};
    busConfig.mosi_io_num = m_config.mosiPin;
    busConfig.miso_io_num = m_config.misoPin;
    busConfig.sclk_io_num = m_config.clockPin;
    busConfig.quadwp_io_num = -1;
    busConfig.quadhd_io_num = -1;
    busConfig.max_transfer_sz = static_cast<int>(m_config.maximumTransferSize);

    esp_err_t result = spi_bus_initialize(m_config.host, &busConfig, SPI_DMA_CH_AUTO);
    if (result != ESP_OK) {
        return result;
    }
    m_busInitialized = true;

    spi_device_interface_config_t deviceConfig{};
    deviceConfig.clock_speed_hz = m_config.clockSpeedHz;
    deviceConfig.mode = m_config.mode;
    deviceConfig.spics_io_num = m_config.chipSelectPin;
    deviceConfig.queue_size = 1;

    result = spi_bus_add_device(m_config.host, &deviceConfig, &m_device);
    if (result != ESP_OK) {
        spi_bus_free(m_config.host);
        m_busInitialized = false;
        return result;
    }

    m_dummyTransmit.assign(m_config.maximumTransferSize, 0U);
    return ESP_OK;
}

esp_err_t SpiMaster::deinitialize()
{
    esp_err_t result = ESP_OK;
    if (m_device != nullptr) {
        result = spi_bus_remove_device(m_device);
        m_device = nullptr;
    }

    if (m_busInitialized) {
        const esp_err_t freeResult = spi_bus_free(m_config.host);
        if (result == ESP_OK) {
            result = freeResult;
        }
        m_busInitialized = false;
    }

    m_dummyTransmit.clear();
    return result;
}

esp_err_t SpiMaster::transfer(const uint8_t* transmit, uint8_t* receive, const size_t length)
{
    if (!isInitialized()) {
        return ESP_ERR_INVALID_STATE;
    }

    if (length == 0U || length > m_config.maximumTransferSize || transmit == nullptr) {
        return ESP_ERR_INVALID_ARG;
    }

    spi_transaction_t transaction{};
    transaction.length = length * 8U;
    transaction.tx_buffer = transmit;
    transaction.rx_buffer = receive;
    return spi_device_transmit(m_device, &transaction);
}

esp_err_t SpiMaster::send(const uint8_t* transmit, const size_t length)
{
    return transfer(transmit, nullptr, length);
}

esp_err_t SpiMaster::receive(uint8_t* receive, const size_t length)
{
    if (receive == nullptr || length == 0U || length > m_dummyTransmit.size()) {
        return ESP_ERR_INVALID_ARG;
    }

    return transfer(m_dummyTransmit.data(), receive, length);
}

bool SpiMaster::isInitialized() const noexcept
{
    return m_device != nullptr;
}

} // namespace control_hub::drivers::spi
