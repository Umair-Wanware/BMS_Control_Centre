#include "communication/communication_manager.hpp"

#include <vector>

#include "communication/packet_builder.hpp"
#include "communication/packet_parser.hpp"
#include "drivers/spi/spi_master.hpp"

namespace control_hub::communication {

CommunicationManager::CommunicationManager(drivers::spi::SpiMaster& spi)
    : CommunicationManager(spi, Config{})
{
}

CommunicationManager::CommunicationManager(drivers::spi::SpiMaster& spi, Config config)
    : m_spi(spi), m_config(config)
{
}

CommunicationManager::~CommunicationManager()
{
    if (m_mutex != nullptr) {
        vSemaphoreDelete(m_mutex);
        m_mutex = nullptr;
    }
}

esp_err_t CommunicationManager::initialize()
{
    if (m_mutex != nullptr) {
        return ESP_OK;
    }

    m_mutex = xSemaphoreCreateMutex();
    return m_mutex == nullptr ? ESP_ERR_NO_MEM : ESP_OK;
}

esp_err_t CommunicationManager::sendPacket(const Packet& packet, Packet* response)
{
    if (m_mutex == nullptr) {
        return ESP_ERR_INVALID_STATE;
    }

    if (xSemaphoreTake(m_mutex, m_config.mutexTimeoutTicks) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }

    const esp_err_t result = exchangeLocked(packet, response);
    xSemaphoreGive(m_mutex);
    return result;
}

esp_err_t CommunicationManager::sendCommand(const Command command, const uint8_t* payload,
                                            const size_t payloadLength)
{
    if (m_mutex == nullptr) {
        return ESP_ERR_INVALID_STATE;
    }

    if (xSemaphoreTake(m_mutex, m_config.mutexTimeoutTicks) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }

    uint16_t sequence = 0U;
    Packet packet{};
    PacketBuilder builder;
    esp_err_t result = nextSequenceLocked(sequence);
    if (result == ESP_OK) {
        result = toEspError(builder.build(command, sequence, payload, payloadLength, packet));
    }
    if (result == ESP_OK) {
        result = exchangeLocked(packet, nullptr);
    }

    xSemaphoreGive(m_mutex);
    return result;
}

esp_err_t CommunicationManager::sendCommandWithAcknowledgement(const Command command,
                                                                const uint8_t* payload,
                                                                const size_t payloadLength,
                                                                Packet* acknowledgement)
{
    if (m_mutex == nullptr) {
        return ESP_ERR_INVALID_STATE;
    }

    if (xSemaphoreTake(m_mutex, m_config.mutexTimeoutTicks) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }

    uint16_t sequence = 0U;
    Packet outbound{};
    PacketBuilder builder;
    esp_err_t result = nextSequenceLocked(sequence);
    if (result == ESP_OK) {
        result = toEspError(builder.build(command, sequence, payload, payloadLength, outbound));
    }

    Packet response{};
    for (uint8_t attempt = 0U; result == ESP_OK && attempt <= m_config.maximumRetries; ++attempt) {
        const esp_err_t exchangeResult = exchangeLocked(outbound, &response);
        if (exchangeResult == ESP_OK && response.sequence == sequence &&
            response.command == Command::Ack) {
            if (acknowledgement != nullptr) {
                *acknowledgement = response;
            }
            xSemaphoreGive(m_mutex);
            return ESP_OK;
        }

        if (exchangeResult == ESP_OK && response.sequence == sequence &&
            response.command == Command::Nack) {
            if (acknowledgement != nullptr) {
                *acknowledgement = response;
            }
            xSemaphoreGive(m_mutex);
            return ESP_FAIL;
        }

        if (attempt < m_config.maximumRetries) {
            vTaskDelay(m_config.retryDelayTicks);
        }
    }

    xSemaphoreGive(m_mutex);
    return ESP_ERR_TIMEOUT;
}

esp_err_t CommunicationManager::receivePacket(Packet& packet)
{
    if (m_mutex == nullptr) {
        return ESP_ERR_INVALID_STATE;
    }

    if (xSemaphoreTake(m_mutex, m_config.mutexTimeoutTicks) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }

    std::vector<uint8_t> receiveBuffer(protocol::kMaximumPacketLength, 0U);
    esp_err_t result = m_spi.receive(receiveBuffer.data(), receiveBuffer.size());
    if (result == ESP_OK) {
        PacketParser parser;
        result = toEspError(parser.parse(receiveBuffer.data(), receiveBuffer.size(), packet));
    }

    xSemaphoreGive(m_mutex);
    return result;
}

esp_err_t CommunicationManager::exchangeLocked(const Packet& packet, Packet* response)
{
    PacketBuilder builder;
    size_t transmitLength = 0U;
    std::vector<uint8_t> transmitBuffer(protocol::kMaximumPacketLength, 0U);
    esp_err_t result = toEspError(builder.serialize(packet, transmitBuffer.data(),
                                                     transmitBuffer.size(), transmitLength));
    if (result != ESP_OK) {
        return result;
    }

    std::vector<uint8_t> receiveBuffer(transmitLength, 0U);
    result = m_spi.transfer(transmitBuffer.data(), receiveBuffer.data(), transmitLength);
    if (result != ESP_OK || response == nullptr) {
        return result;
    }

    PacketParser parser;
    return toEspError(parser.parse(receiveBuffer.data(), receiveBuffer.size(), *response));
}

esp_err_t CommunicationManager::nextSequenceLocked(uint16_t& sequence) noexcept
{
    sequence = m_nextSequence++;
    if (m_nextSequence == 0U) {
        m_nextSequence = 1U;
    }
    return ESP_OK;
}

esp_err_t CommunicationManager::toEspError(const protocol::ValidationError error) noexcept
{
    switch (error) {
    case protocol::ValidationError::None:
        return ESP_OK;
    case protocol::ValidationError::CrcMismatch:
        return ESP_ERR_INVALID_CRC;
    case protocol::ValidationError::InsufficientData:
    case protocol::ValidationError::InvalidPacketLength:
        return ESP_ERR_INVALID_SIZE;
    case protocol::ValidationError::InvalidHeader:
    case protocol::ValidationError::UnsupportedVersion:
    case protocol::ValidationError::InvalidCommand:
    case protocol::ValidationError::PayloadTooLarge:
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_FAIL;
}

} // namespace control_hub::communication
