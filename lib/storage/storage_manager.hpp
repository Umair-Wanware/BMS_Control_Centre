#pragma once

#include <stddef.h>
#include <stdint.h>

#include <string>

#include "esp_err.h"

namespace control_hub::drivers::filesystem {
class FileSystemManager;
}

/** @file storage_manager.hpp @brief Persistent settings and firmware-image storage. */

namespace control_hub::storage {

/**
 * @brief Persisted network settings consumed by the application during boot.
 */
struct NetworkSettings {
    bool useStationMode = false;
    char stationSsid[33]{};
    char stationPassword[65]{};
    char accessPointSsid[33] = "BMS-Control-Hub";
    char accessPointPassword[65]{};
};

/**
 * @brief Stores configuration and uploaded images on the mounted LittleFS partition.
 */
class StorageManager final {
public:
    explicit StorageManager(drivers::filesystem::FileSystemManager& fileSystem);

    /** Loads saved settings, returning defaults when no settings file exists. */
    esp_err_t loadNetworkSettings(NetworkSettings& settings) const;

    /** Validates and atomically saves network settings. */
    esp_err_t saveNetworkSettings(const NetworkSettings& settings) const;

    /** Stores an uploaded firmware image under a safe LittleFS filename. */
    esp_err_t saveFirmwareImage(const char* filename, const uint8_t* image,
                                size_t imageLength, std::string& storedPath) const;

    /** Deletes a previously stored firmware image. */
    esp_err_t removeFirmwareImage(const char* filename) const;

private:
    static bool isValidText(const char* value, size_t maximumLength) noexcept;
    static bool isValidFilename(const char* filename) noexcept;

    drivers::filesystem::FileSystemManager& m_fileSystem;
};

} // namespace control_hub::storage
