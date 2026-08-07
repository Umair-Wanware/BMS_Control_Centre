#pragma once

#include <stdint.h>

/**
 * @file commands.hpp
 * @brief Command identifiers exchanged between the ESP32 Control Hub and STM32 BMS.
 *
 * The numeric values are part of the wire protocol and must remain stable once
 * firmware has been deployed to either device.
 */

namespace control_hub::communication {

/**
 * @brief Identifies the purpose of a protocol packet.
 *
 * Commands are grouped by function to make packet traces easier to inspect and
 * to leave room for protocol extensions. A value that is not listed here is
 * invalid and must be rejected by the packet parser.
 */
enum class Command : uint8_t {
    None = 0x00,

    Heartbeat = 0x01,
    Ack = 0x02,
    Nack = 0x03,
    Reset = 0x04,

    RequestTelemetry = 0x10,
    Telemetry = 0x11,

    StartUpdate = 0x20,
    FirmwareHeader = 0x21,
    FirmwareData = 0x22,
    EndUpdate = 0x23,
    VerifyImage = 0x24,

    GetConfiguration = 0x30,
    SetConfiguration = 0x31,

    Ping = 0x40,
    Pong = 0x41,
    GetVersion = 0x42,
    GetDeviceInfo = 0x43,

    Error = 0xFF,
};

/**
 * @brief Returns whether a raw command byte belongs to the current protocol.
 *
 * @param value Command byte read from a packet.
 * @return true when @p value maps to a defined Command; otherwise false.
 */
constexpr bool isValidCommand(const uint8_t value) noexcept
{
    switch (static_cast<Command>(value)) {
    case Command::None:
    case Command::Heartbeat:
    case Command::Ack:
    case Command::Nack:
    case Command::Reset:
    case Command::RequestTelemetry:
    case Command::Telemetry:
    case Command::StartUpdate:
    case Command::FirmwareHeader:
    case Command::FirmwareData:
    case Command::EndUpdate:
    case Command::VerifyImage:
    case Command::GetConfiguration:
    case Command::SetConfiguration:
    case Command::Ping:
    case Command::Pong:
    case Command::GetVersion:
    case Command::GetDeviceInfo:
    case Command::Error:
        return true;
    }

    return false;
}

/**
 * @brief Returns whether a command acknowledges or rejects another packet.
 */
constexpr bool isAcknowledgementCommand(const Command command) noexcept
{
    return command == Command::Ack || command == Command::Nack;
}

/**
 * @brief Returns whether a command belongs to the STM32 firmware-update flow.
 */
constexpr bool isFirmwareUpdateCommand(const Command command) noexcept
{
    switch (command) {
    case Command::StartUpdate:
    case Command::FirmwareHeader:
    case Command::FirmwareData:
    case Command::EndUpdate:
    case Command::VerifyImage:
        return true;

    default:
        return false;
    }
}

} // namespace control_hub::communication
