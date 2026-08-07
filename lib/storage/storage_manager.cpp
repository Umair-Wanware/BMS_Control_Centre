#include "storage/storage_manager.hpp"

#include <stdio.h>
#include <string.h>

#include <vector>

#include "drivers/filesystem/filesystem_manager.hpp"

namespace control_hub::storage {
namespace {

constexpr char kSettingsPath[] = "/settings.cfg";
constexpr char kFirmwarePrefix[] = "/firmware_";

const char* findSetting(const char* settings, const char* key) noexcept
{
    const size_t keyLength = strlen(key);
    const char* current = settings;
    while (*current != '\0') {
        if (strncmp(current, key, keyLength) == 0 && current[keyLength] == '=') {
            return current + keyLength + 1U;
        }
        const char* lineEnd = strchr(current, '\n');
        if (lineEnd == nullptr) {
            break;
        }
        current = lineEnd + 1U;
    }
    return nullptr;
}

bool readSetting(const char* settings, const char* key, char* destination,
                 const size_t capacity) noexcept
{
    const char* value = findSetting(settings, key);
    if (value == nullptr) {
        return false;
    }

    const char* lineEnd = strchr(value, '\n');
    const size_t length = lineEnd == nullptr ? strlen(value) : static_cast<size_t>(lineEnd - value);
    if (length >= capacity) {
        return false;
    }

    memcpy(destination, value, length);
    destination[length] = '\0';
    return true;
}

} // namespace

StorageManager::StorageManager(drivers::filesystem::FileSystemManager& fileSystem)
    : m_fileSystem(fileSystem)
{
}

esp_err_t StorageManager::loadNetworkSettings(NetworkSettings& settings) const
{
    settings = NetworkSettings{};

    std::vector<uint8_t> contents;
    const esp_err_t result = m_fileSystem.readFile(kSettingsPath, contents);
    if (result == ESP_ERR_NOT_FOUND) {
        return ESP_OK;
    }
    if (result != ESP_OK) {
        return result;
    }

    contents.push_back('\0');
    const char* rawSettings = reinterpret_cast<const char*>(contents.data());
    char mode[8]{};
    if (!readSetting(rawSettings, "mode", mode, sizeof(mode)) ||
        !readSetting(rawSettings, "sta_ssid", settings.stationSsid, sizeof(settings.stationSsid)) ||
        !readSetting(rawSettings, "sta_password", settings.stationPassword,
                     sizeof(settings.stationPassword)) ||
        !readSetting(rawSettings, "ap_ssid", settings.accessPointSsid,
                     sizeof(settings.accessPointSsid)) ||
        !readSetting(rawSettings, "ap_password", settings.accessPointPassword,
                     sizeof(settings.accessPointPassword))) {
        return ESP_ERR_INVALID_RESPONSE;
    }

    if (strcmp(mode, "sta") == 0) {
        settings.useStationMode = true;
    } else if (strcmp(mode, "ap") == 0) {
        settings.useStationMode = false;
    } else {
        return ESP_ERR_INVALID_RESPONSE;
    }

    return saveNetworkSettings(settings) == ESP_OK ? ESP_OK : ESP_ERR_INVALID_RESPONSE;
}

esp_err_t StorageManager::saveNetworkSettings(const NetworkSettings& settings) const
{
    if (!isValidText(settings.stationSsid, sizeof(settings.stationSsid)) ||
        !isValidText(settings.stationPassword, sizeof(settings.stationPassword)) ||
        !isValidText(settings.accessPointSsid, sizeof(settings.accessPointSsid)) ||
        !isValidText(settings.accessPointPassword, sizeof(settings.accessPointPassword)) ||
        (settings.useStationMode && settings.stationSsid[0] == '\0') ||
        settings.accessPointSsid[0] == '\0') {
        return ESP_ERR_INVALID_ARG;
    }

    char serialized[256]{};
    const int written = snprintf(serialized, sizeof(serialized),
                                 "mode=%s\nsta_ssid=%s\nsta_password=%s\nap_ssid=%s\nap_password=%s\n",
                                 settings.useStationMode ? "sta" : "ap", settings.stationSsid,
                                 settings.stationPassword, settings.accessPointSsid,
                                 settings.accessPointPassword);
    if (written < 0 || static_cast<size_t>(written) >= sizeof(serialized)) {
        return ESP_ERR_INVALID_SIZE;
    }

    return m_fileSystem.writeFile(kSettingsPath,
                                  reinterpret_cast<const uint8_t*>(serialized),
                                  static_cast<size_t>(written));
}

esp_err_t StorageManager::saveFirmwareImage(const char* filename, const uint8_t* image,
                                            const size_t imageLength,
                                            std::string& storedPath) const
{
    storedPath.clear();
    if (!isValidFilename(filename) || (image == nullptr && imageLength != 0U)) {
        return ESP_ERR_INVALID_ARG;
    }

    storedPath = std::string(kFirmwarePrefix) + filename;
    return m_fileSystem.writeFile(storedPath.c_str(), image, imageLength);
}

esp_err_t StorageManager::removeFirmwareImage(const char* filename) const
{
    if (!isValidFilename(filename)) {
        return ESP_ERR_INVALID_ARG;
    }

    const std::string path = std::string(kFirmwarePrefix) + filename;
    return m_fileSystem.removeFile(path.c_str());
}

bool StorageManager::isValidText(const char* value, const size_t maximumLength) noexcept
{
    if (value == nullptr || strnlen(value, maximumLength) >= maximumLength) {
        return false;
    }

    return strchr(value, '\n') == nullptr && strchr(value, '\r') == nullptr;
}

bool StorageManager::isValidFilename(const char* filename) noexcept
{
    if (filename == nullptr || filename[0] == '\0' || strchr(filename, '/') != nullptr ||
        strstr(filename, "..") != nullptr || strnlen(filename, 65U) >= 65U) {
        return false;
    }

    for (const char* current = filename; *current != '\0'; ++current) {
        const bool valid = (*current >= 'a' && *current <= 'z') ||
                           (*current >= 'A' && *current <= 'Z') ||
                           (*current >= '0' && *current <= '9') || *current == '.' ||
                           *current == '_' || *current == '-';
        if (!valid) {
            return false;
        }
    }
    return true;
}

} // namespace control_hub::storage
