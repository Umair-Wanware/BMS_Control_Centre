#include "updater/firmware_updater.hpp"

#include <array>
#include <limits.h>
#include <string.h>

#include "communication/commands.hpp"
#include "communication/communication_manager.hpp"
#include "communication/crc32.hpp"
#include "communication/protocol.hpp"
#include "drivers/filesystem/filesystem_manager.hpp"

namespace control_hub::updater {
namespace {

constexpr size_t kChecksumReadLength = 1024U;
constexpr size_t kFirmwareDataOffsetLength = sizeof(uint32_t);
constexpr size_t kFirmwareDataLength =
    communication::protocol::kMaximumPayloadLength - kFirmwareDataOffsetLength;

void writeUint16Le(uint8_t* destination, const uint16_t value) noexcept
{
    destination[0] = static_cast<uint8_t>(value & 0xFFU);
    destination[1] = static_cast<uint8_t>((value >> 8U) & 0xFFU);
}

void writeUint32Le(uint8_t* destination, const uint32_t value) noexcept
{
    destination[0] = static_cast<uint8_t>(value & 0xFFU);
    destination[1] = static_cast<uint8_t>((value >> 8U) & 0xFFU);
    destination[2] = static_cast<uint8_t>((value >> 16U) & 0xFFU);
    destination[3] = static_cast<uint8_t>((value >> 24U) & 0xFFU);
}

} // namespace

FirmwareUpdater::FirmwareUpdater(communication::CommunicationManager& communication,
                                 drivers::filesystem::FileSystemManager& fileSystem)
    : m_communication(communication), m_fileSystem(fileSystem)
{
    m_mutex = xSemaphoreCreateMutex();
}

FirmwareUpdater::~FirmwareUpdater()
{
    cancelUpdate();
    if (m_mutex != nullptr && m_task == nullptr) {
        vSemaphoreDelete(m_mutex);
        m_mutex = nullptr;
    }
}

esp_err_t FirmwareUpdater::startUpdate(const char* imagePath)
{
    if (imagePath == nullptr || imagePath[0] != '/' || strstr(imagePath, "..") != nullptr ||
        m_mutex == nullptr) {
        return ESP_ERR_INVALID_ARG;
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(1000U)) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }

    if (m_task != nullptr) {
        xSemaphoreGive(m_mutex);
        return ESP_ERR_INVALID_STATE;
    }

    m_imagePath = imagePath;
    m_cancelRequested = false;
    m_progress = FirmwareUpdateProgress{};
    m_progress.state = FirmwareUpdateState::CalculatingChecksum;
    const BaseType_t created = xTaskCreate(&FirmwareUpdater::taskEntry, "firmware_update",
                                           6144U, this, 5U, &m_task);
    if (created != pdPASS) {
        m_task = nullptr;
        m_progress.state = FirmwareUpdateState::Failed;
        m_progress.lastError = ESP_ERR_NO_MEM;
        xSemaphoreGive(m_mutex);
        return ESP_ERR_NO_MEM;
    }

    xSemaphoreGive(m_mutex);
    return ESP_OK;
}

esp_err_t FirmwareUpdater::cancelUpdate()
{
    if (m_mutex == nullptr) {
        return ESP_ERR_INVALID_STATE;
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(1000U)) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }

    if (m_task == nullptr) {
        xSemaphoreGive(m_mutex);
        return ESP_ERR_INVALID_STATE;
    }

    m_cancelRequested = true;
    xSemaphoreGive(m_mutex);
    return ESP_OK;
}

FirmwareUpdateProgress FirmwareUpdater::progress() const
{
    FirmwareUpdateProgress snapshot{};
    if (m_mutex != nullptr && xSemaphoreTake(m_mutex, pdMS_TO_TICKS(1000U)) == pdTRUE) {
        snapshot = m_progress;
        xSemaphoreGive(m_mutex);
    }
    return snapshot;
}

bool FirmwareUpdater::isRunning() const
{
    if (m_mutex == nullptr || xSemaphoreTake(m_mutex, pdMS_TO_TICKS(1000U)) != pdTRUE) {
        return false;
    }

    const bool running = m_task != nullptr;
    xSemaphoreGive(m_mutex);
    return running;
}

void FirmwareUpdater::taskEntry(void* argument)
{
    static_cast<FirmwareUpdater*>(argument)->runUpdate();
    vTaskDelete(nullptr);
}

void FirmwareUpdater::runUpdate()
{
    size_t fileSize = 0U;
    esp_err_t result = m_fileSystem.getFileSize(m_imagePath.c_str(), fileSize);
    if (result == ESP_OK && (fileSize == 0U || fileSize > UINT32_MAX)) {
        result = ESP_ERR_INVALID_SIZE;
    }

    uint32_t crc32 = 0U;
    if (result == ESP_OK) {
        setProgress(FirmwareUpdateState::CalculatingChecksum, static_cast<uint32_t>(fileSize),
                    0U, 0U, ESP_OK);
        result = calculateImageCrc(m_imagePath.c_str(), static_cast<uint32_t>(fileSize), crc32);
    }

    if (result == ESP_OK && cancellationRequested()) {
        result = ESP_ERR_INVALID_STATE;
        setProgress(FirmwareUpdateState::Cancelled, static_cast<uint32_t>(fileSize), 0U, crc32,
                    result);
    }

    if (result == ESP_OK) {
        result = transferImage(m_imagePath.c_str(), static_cast<uint32_t>(fileSize), crc32);
    }

    if (result == ESP_OK) {
        setProgress(FirmwareUpdateState::Complete, static_cast<uint32_t>(fileSize),
                    static_cast<uint32_t>(fileSize), crc32, ESP_OK);
    } else if (progress().state != FirmwareUpdateState::Cancelled) {
        setProgress(FirmwareUpdateState::Failed, static_cast<uint32_t>(fileSize),
                    progress().transferredBytes, crc32, result);
    }

    if (xSemaphoreTake(m_mutex, pdMS_TO_TICKS(1000U)) == pdTRUE) {
        m_task = nullptr;
        xSemaphoreGive(m_mutex);
    }
}

esp_err_t FirmwareUpdater::calculateImageCrc(const char* imagePath, const uint32_t imageSize,
                                             uint32_t& crc32)
{
    std::array<uint8_t, kChecksumReadLength> buffer{};
    uint32_t offset = 0U;
    uint32_t runningCrc = communication::Crc32::initialize();

    while (offset < imageSize) {
        if (cancellationRequested()) {
            return ESP_ERR_INVALID_STATE;
        }

        size_t bytesRead = 0U;
        const size_t requested =
            imageSize - offset < buffer.size() ? imageSize - offset : buffer.size();
        const esp_err_t result = m_fileSystem.readFileChunk(imagePath, offset, buffer.data(),
                                                             requested, bytesRead);
        if (result != ESP_OK || bytesRead != requested) {
            return result == ESP_OK ? ESP_ERR_INVALID_SIZE : result;
        }

        runningCrc = communication::Crc32::update(runningCrc, buffer.data(), bytesRead);
        offset += static_cast<uint32_t>(bytesRead);
    }

    crc32 = communication::Crc32::finalize(runningCrc);
    return ESP_OK;
}

esp_err_t FirmwareUpdater::transferImage(const char* imagePath, const uint32_t imageSize,
                                         const uint32_t crc32)
{
    uint8_t header[10]{};
    writeUint32Le(&header[0], imageSize);
    writeUint32Le(&header[4], crc32);
    writeUint16Le(&header[8], static_cast<uint16_t>(kFirmwareDataLength));

    setProgress(FirmwareUpdateState::Starting, imageSize, 0U, crc32, ESP_OK);
    esp_err_t result = m_communication.sendCommandWithAcknowledgement(
        communication::Command::StartUpdate, nullptr, 0U);
    if (result != ESP_OK) {
        return result;
    }
    result = m_communication.sendCommandWithAcknowledgement(
        communication::Command::FirmwareHeader, header, sizeof(header));
    if (result != ESP_OK) {
        return result;
    }

    std::array<uint8_t, communication::protocol::kMaximumPayloadLength> packetPayload{};
    uint32_t offset = 0U;
    while (offset < imageSize) {
        if (cancellationRequested()) {
            return ESP_ERR_INVALID_STATE;
        }

        const size_t requested = imageSize - offset < kFirmwareDataLength
                                     ? imageSize - offset
                                     : kFirmwareDataLength;
        size_t bytesRead = 0U;
        result = m_fileSystem.readFileChunk(imagePath, offset,
                                            &packetPayload[kFirmwareDataOffsetLength], requested,
                                            bytesRead);
        if (result != ESP_OK || bytesRead != requested) {
            return result == ESP_OK ? ESP_ERR_INVALID_SIZE : result;
        }

        writeUint32Le(packetPayload.data(), offset);
        result = m_communication.sendCommandWithAcknowledgement(
            communication::Command::FirmwareData, packetPayload.data(),
            kFirmwareDataOffsetLength + bytesRead);
        if (result != ESP_OK) {
            return result;
        }

        offset += static_cast<uint32_t>(bytesRead);
        setProgress(FirmwareUpdateState::Transferring, imageSize, offset, crc32, ESP_OK);
    }

    uint8_t completion[8]{};
    writeUint32Le(&completion[0], imageSize);
    writeUint32Le(&completion[4], crc32);
    result = m_communication.sendCommandWithAcknowledgement(
        communication::Command::EndUpdate, completion, sizeof(completion));
    if (result != ESP_OK) {
        return result;
    }

    setProgress(FirmwareUpdateState::Verifying, imageSize, imageSize, crc32, ESP_OK);
    return m_communication.sendCommandWithAcknowledgement(
        communication::Command::VerifyImage, nullptr, 0U);
}

void FirmwareUpdater::setProgress(const FirmwareUpdateState state, const uint32_t imageSize,
                                  const uint32_t transferredBytes, const uint32_t imageCrc32,
                                  const esp_err_t error) noexcept
{
    if (m_mutex != nullptr && xSemaphoreTake(m_mutex, pdMS_TO_TICKS(1000U)) == pdTRUE) {
        m_progress.state = state;
        m_progress.imageSizeBytes = imageSize;
        m_progress.transferredBytes = transferredBytes;
        m_progress.imageCrc32 = imageCrc32;
        m_progress.lastError = error;
        xSemaphoreGive(m_mutex);
    }
}

bool FirmwareUpdater::cancellationRequested() const noexcept
{
    if (m_mutex == nullptr || xSemaphoreTake(m_mutex, pdMS_TO_TICKS(1000U)) != pdTRUE) {
        return true;
    }

    const bool requested = m_cancelRequested;
    xSemaphoreGive(m_mutex);
    return requested;
}

} // namespace control_hub::updater
