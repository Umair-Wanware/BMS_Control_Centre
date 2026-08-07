#pragma once

#include <stddef.h>
#include <stdint.h>

#include <string>

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

namespace control_hub::communication {
class CommunicationManager;
}
namespace control_hub::drivers::filesystem {
class FileSystemManager;
}

/** @file firmware_updater.hpp @brief Reliable STM32 firmware-update workflow. */

namespace control_hub::updater {

/** @brief State of a firmware-update request. */
enum class FirmwareUpdateState : uint8_t {
    Idle,
    CalculatingChecksum,
    Starting,
    Transferring,
    Verifying,
    Complete,
    Failed,
    Cancelled,
};

/** @brief Snapshot of firmware-update progress for dashboard consumers. */
struct FirmwareUpdateProgress {
    FirmwareUpdateState state = FirmwareUpdateState::Idle;
    uint32_t imageSizeBytes = 0U;
    uint32_t transferredBytes = 0U;
    uint32_t imageCrc32 = 0U;
    esp_err_t lastError = ESP_OK;
};

/**
 * @brief Streams a LittleFS image to the STM32 using acknowledged BMS packets.
 */
class FirmwareUpdater final {
public:
    FirmwareUpdater(communication::CommunicationManager& communication,
                    drivers::filesystem::FileSystemManager& fileSystem);
    ~FirmwareUpdater();

    FirmwareUpdater(const FirmwareUpdater&) = delete;
    FirmwareUpdater& operator=(const FirmwareUpdater&) = delete;

    /** Starts a non-blocking firmware update from a LittleFS-relative image path. */
    esp_err_t startUpdate(const char* imagePath);

    /** Requests cancellation at the next safe packet boundary. */
    esp_err_t cancelUpdate();

    /** Returns a coherent progress snapshot. */
    FirmwareUpdateProgress progress() const;

    /** Returns whether an update task is currently running. */
    bool isRunning() const;

private:
    static void taskEntry(void* argument);
    void runUpdate();
    esp_err_t calculateImageCrc(const char* imagePath, uint32_t imageSize,
                                uint32_t& crc32);
    esp_err_t transferImage(const char* imagePath, uint32_t imageSize, uint32_t crc32);
    void setProgress(FirmwareUpdateState state, uint32_t imageSize,
                     uint32_t transferredBytes, uint32_t imageCrc32,
                     esp_err_t error) noexcept;
    bool cancellationRequested() const noexcept;

    communication::CommunicationManager& m_communication;
    drivers::filesystem::FileSystemManager& m_fileSystem;
    mutable SemaphoreHandle_t m_mutex = nullptr;
    TaskHandle_t m_task = nullptr;
    FirmwareUpdateProgress m_progress{};
    std::string m_imagePath;
    bool m_cancelRequested = false;
};

} // namespace control_hub::updater
