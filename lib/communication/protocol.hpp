#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * @file protocol.hpp
 * @brief Versioned constants and validation helpers for the BMS SPI protocol.
 */

namespace control_hub::communication::protocol {

/**
 * @brief Current version of the Control Hub to BMS packet protocol.
 *
 * The protocol version is encoded in the low nibble of every packet header.
 */
constexpr uint8_t kCurrentVersion = 0x01;

/** @brief Maximum protocol version representable in a packet header. */
constexpr uint8_t kMaximumVersion = 0x0F;

/**
 * @brief Header magic value with the protocol-version bits cleared.
 *
 * The lower four bits are reserved for the protocol version. The generated
 * header for protocol version 1 is therefore 0xA5C1.
 */
constexpr uint16_t kHeaderMagic = 0xA5C0;

/** @brief Mask selecting the fixed magic bits of a packet header. */
constexpr uint16_t kHeaderMagicMask = 0xFFF0;

/** @brief Mask selecting the protocol-version bits of a packet header. */
constexpr uint16_t kHeaderVersionMask = 0x000F;

/** @brief Maximum number of application payload bytes in one packet. */
constexpr size_t kMaximumPayloadLength = 128U;

/** @brief Number of bytes occupied by the header field on the wire. */
constexpr size_t kHeaderFieldLength = sizeof(uint16_t);

/** @brief Number of bytes occupied by the command field on the wire. */
constexpr size_t kCommandFieldLength = sizeof(uint8_t);

/** @brief Number of bytes occupied by the sequence field on the wire. */
constexpr size_t kSequenceFieldLength = sizeof(uint16_t);

/** @brief Number of bytes occupied by the payload-length field on the wire. */
constexpr size_t kPayloadLengthFieldLength = sizeof(uint16_t);

/** @brief Number of bytes occupied by the packet CRC16 field on the wire. */
constexpr size_t kPacketCrcLength = sizeof(uint16_t);

/** @brief Number of bytes occupied by a firmware-image CRC32 value. */
constexpr size_t kFirmwareCrcLength = sizeof(uint32_t);

/** @brief Number of packet bytes preceding a variable-length payload. */
constexpr size_t kPacketPrefixLength =
    kHeaderFieldLength + kCommandFieldLength + kSequenceFieldLength +
    kPayloadLengthFieldLength;

/** @brief Minimum valid serialized packet length. */
constexpr size_t kMinimumPacketLength = kPacketPrefixLength + kPacketCrcLength;

/** @brief Maximum valid serialized packet length. */
constexpr size_t kMaximumPacketLength =
    kMinimumPacketLength + kMaximumPayloadLength;

/**
 * @brief Identifies packet validation failures shared by protocol components.
 */
enum class ValidationError : uint8_t {
    None = 0,
    InsufficientData,
    InvalidHeader,
    UnsupportedVersion,
    InvalidCommand,
    PayloadTooLarge,
    InvalidPacketLength,
    CrcMismatch,
};

/**
 * @brief Builds a packet header containing the specified protocol version.
 *
 * @param version Protocol version to encode.
 * @return A valid wire header, or zero when @p version is not representable.
 */
constexpr uint16_t makeHeader(const uint8_t version = kCurrentVersion) noexcept
{
    return version <= kMaximumVersion
               ? static_cast<uint16_t>(kHeaderMagic | version)
               : 0U;
}

/**
 * @brief Returns whether a header contains the expected protocol magic bits.
 */
constexpr bool hasValidHeaderMagic(const uint16_t header) noexcept
{
    return (header & kHeaderMagicMask) == kHeaderMagic;
}

/**
 * @brief Extracts the protocol version encoded in a packet header.
 */
constexpr uint8_t getVersion(const uint16_t header) noexcept
{
    return static_cast<uint8_t>(header & kHeaderVersionMask);
}

/**
 * @brief Returns whether the supplied protocol version is supported locally.
 */
constexpr bool isSupportedVersion(const uint8_t version) noexcept
{
    return version == kCurrentVersion;
}

/**
 * @brief Returns whether a payload length can be represented by this protocol.
 */
constexpr bool isValidPayloadLength(const size_t length) noexcept
{
    return length <= kMaximumPayloadLength;
}

/**
 * @brief Returns the serialized packet length for a valid payload length.
 *
 * @return The packet length, or zero when @p payloadLength exceeds the limit.
 */
constexpr size_t serializedPacketLength(const size_t payloadLength) noexcept
{
    return isValidPayloadLength(payloadLength)
               ? kMinimumPacketLength + payloadLength
               : 0U;
}

/**
 * @brief Returns whether a byte count is a possible serialized packet length.
 */
constexpr bool isValidSerializedPacketLength(const size_t packetLength) noexcept
{
    return packetLength >= kMinimumPacketLength &&
           packetLength <= kMaximumPacketLength;
}

} // namespace control_hub::communication::protocol
