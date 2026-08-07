#pragma once

#include <stddef.h>
#include <stdint.h>

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "communication/packet.hpp"

namespace control_hub::drivers::spi {
class SpiMaster;
}

/** @file communication_manager.hpp @brief Reliable command exchange over SPI. */

namespace control_hub::communication {

/**
 * @brief Coordinates packet serialization, SPI transfers, ACK handling, and retries.
 */
class CommunicationManager final {
public:
    /** @brief Retry and synchronization policy for BMS command exchanges. */
    struct Config {
        uint8_t maximumRetries = 3U;
        TickType_t retryDelayTicks = pdMS_TO_TICKS(25U);
        TickType_t mutexTimeoutTicks = pdMS_TO_TICKS(1000U);
    };

    explicit CommunicationManager(drivers::spi::SpiMaster& spi);
    CommunicationManager(drivers::spi::SpiMaster& spi, Config config);
    ~CommunicationManager();

    CommunicationManager(const CommunicationManager&) = delete;
    CommunicationManager& operator=(const CommunicationManager&) = delete;

    /** Creates the synchronization resources required for thread-safe transfers. */
    esp_err_t initialize();

    /** Sends a pre-built packet and optionally decodes a simultaneous response. */
    esp_err_t sendPacket(const Packet& packet, Packet* response = nullptr);

    /** Builds and sends a command without requiring acknowledgement. */
    esp_err_t sendCommand(Command command, const uint8_t* payload, size_t payloadLength);

    /** Sends a command and retries until the BMS replies with ACK or NACK. */
    esp_err_t sendCommandWithAcknowledgement(Command command, const uint8_t* payload,
                                              size_t payloadLength,
                                              Packet* acknowledgement = nullptr);

    /** Clocks a maximum-sized receive frame and validates the returned packet. */
    esp_err_t receivePacket(Packet& packet);

private:
    esp_err_t exchangeLocked(const Packet& packet, Packet* response);
    esp_err_t nextSequenceLocked(uint16_t& sequence) noexcept;
    static esp_err_t toEspError(protocol::ValidationError error) noexcept;

    drivers::spi::SpiMaster& m_spi;
    Config m_config;
    SemaphoreHandle_t m_mutex = nullptr;
    uint16_t m_nextSequence = 1U;
};

} // namespace control_hub::communication
