#include "drivers/filesystem/filesystem_manager.hpp"

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "esp_littlefs.h"


namespace control_hub::drivers::filesystem {

FileSystemManager::FileSystemManager() : FileSystemManager(Config{}) {}

FileSystemManager::FileSystemManager(Config config) : m_config(config) {}

FileSystemManager::~FileSystemManager()
{
    deinitialize();
}

esp_err_t FileSystemManager::initialize()
{
    if (m_mounted) {
        return ESP_OK;
    }

    if (m_config.basePath == nullptr || m_config.partitionLabel == nullptr) {
        return ESP_ERR_INVALID_ARG;
    }

    m_mutex = xSemaphoreCreateMutex();
    if (m_mutex == nullptr) {
        return ESP_ERR_NO_MEM;
    }

    esp_vfs_littlefs_conf_t configuration{};
    configuration.base_path = m_config.basePath;
    configuration.partition_label = m_config.partitionLabel;
    configuration.format_if_mount_failed = m_config.formatIfMountFailed;
    configuration.dont_mount = false;

    const esp_err_t result = esp_vfs_littlefs_register(&configuration);
    if (result != ESP_OK) {
        vSemaphoreDelete(m_mutex);
        m_mutex = nullptr;
        return result;
    }

    m_mounted = true;
    return ESP_OK;
}

esp_err_t FileSystemManager::deinitialize()
{
    esp_err_t result = ESP_OK;
    if (m_mounted) {
        result = esp_vfs_littlefs_unregister(m_config.partitionLabel);
        m_mounted = false;
    }

    if (m_mutex != nullptr) {
        vSemaphoreDelete(m_mutex);
        m_mutex = nullptr;
    }

    return result;
}

esp_err_t FileSystemManager::readFile(const char* relativePath,
                                      std::vector<uint8_t>& contents) const
{
    size_t fileSize = 0U;
    esp_err_t result = getFileSize(relativePath, fileSize);
    if (result != ESP_OK) {
        return result;
    }

    contents.assign(fileSize, 0U);
    size_t bytesRead = 0U;
    result = readFileChunk(relativePath, 0U, contents.data(), contents.size(), bytesRead);
    if (result != ESP_OK) {
        contents.clear();
        return result;
    }

    contents.resize(bytesRead);
    return ESP_OK;
}

esp_err_t FileSystemManager::readFileChunk(const char* relativePath, const size_t offset,
                                           uint8_t* buffer, const size_t bufferCapacity,
                                           size_t& bytesRead) const
{
    bytesRead = 0U;
    if (!m_mounted) {
        return ESP_ERR_INVALID_STATE;
    }

    if (!isSafeRelativePath(relativePath) || (buffer == nullptr && bufferCapacity != 0U) ||
        offset > static_cast<size_t>(LONG_MAX)) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!takeLock()) {
        return ESP_ERR_TIMEOUT;
    }

    const std::string absolutePath = makeAbsolutePath(relativePath);
    FILE* file = fopen(absolutePath.c_str(), "rb");
    if (file == nullptr) {
        giveLock();
        return ESP_ERR_NOT_FOUND;
    }

    if (fseek(file, static_cast<long>(offset), SEEK_SET) != 0) {
        fclose(file);
        giveLock();
        return ESP_ERR_INVALID_ARG;
    }

    bytesRead = bufferCapacity == 0U ? 0U : fread(buffer, 1U, bufferCapacity, file);
    const bool failed = ferror(file) != 0;
    fclose(file);
    giveLock();
    return failed ? ESP_FAIL : ESP_OK;
}

esp_err_t FileSystemManager::writeFile(const char* relativePath, const uint8_t* data,
                                       const size_t length) const
{
    if (!m_mounted) {
        return ESP_ERR_INVALID_STATE;
    }

    if (!isSafeRelativePath(relativePath) || (data == nullptr && length != 0U)) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!takeLock()) {
        return ESP_ERR_TIMEOUT;
    }

    const std::string absolutePath = makeAbsolutePath(relativePath);
    const std::string temporaryPath = absolutePath + ".tmp";
    FILE* file = fopen(temporaryPath.c_str(), "wb");
    if (file == nullptr) {
        giveLock();
        return ESP_FAIL;
    }

    const size_t bytesWritten = length == 0U ? 0U : fwrite(data, 1U, length, file);
    const bool failed = bytesWritten != length || fflush(file) != 0 || fclose(file) != 0;
    if (failed) {
        remove(temporaryPath.c_str());
        giveLock();
        return ESP_FAIL;
    }

    if (rename(temporaryPath.c_str(), absolutePath.c_str()) != 0) {
        remove(temporaryPath.c_str());
        giveLock();
        return ESP_FAIL;
    }

    giveLock();
    return ESP_OK;
}

esp_err_t FileSystemManager::removeFile(const char* relativePath) const
{
    if (!m_mounted) {
        return ESP_ERR_INVALID_STATE;
    }

    if (!isSafeRelativePath(relativePath)) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!takeLock()) {
        return ESP_ERR_TIMEOUT;
    }

    const std::string absolutePath = makeAbsolutePath(relativePath);
    const int removeResult = remove(absolutePath.c_str());
    giveLock();
    return removeResult == 0 ? ESP_OK : ESP_ERR_NOT_FOUND;
}

esp_err_t FileSystemManager::getFileSize(const char* relativePath, size_t& size) const
{
    size = 0U;
    if (!m_mounted) {
        return ESP_ERR_INVALID_STATE;
    }

    if (!isSafeRelativePath(relativePath)) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!takeLock()) {
        return ESP_ERR_TIMEOUT;
    }

    struct stat information {};
    const std::string absolutePath = makeAbsolutePath(relativePath);
    const int statResult = stat(absolutePath.c_str(), &information);
    giveLock();
    if (statResult != 0 || information.st_size < 0) {
        return ESP_ERR_NOT_FOUND;
    }

    size = static_cast<size_t>(information.st_size);
    return ESP_OK;
}

esp_err_t FileSystemManager::format()
{
    if (!m_mounted) {
        return ESP_ERR_INVALID_STATE;
    }

    if (!takeLock()) {
        return ESP_ERR_TIMEOUT;
    }

    const esp_err_t result = esp_littlefs_format(m_config.partitionLabel);
    giveLock();
    return result;
}

bool FileSystemManager::isMounted() const noexcept
{
    return m_mounted;
}

const char* FileSystemManager::basePath() const noexcept
{
    return m_config.basePath;
}

bool FileSystemManager::isSafeRelativePath(const char* relativePath) const noexcept
{
    return relativePath != nullptr && relativePath[0] == '/' &&
           strstr(relativePath, "..") == nullptr;
}

std::string FileSystemManager::makeAbsolutePath(const char* relativePath) const
{
    return std::string(m_config.basePath) + relativePath;
}

bool FileSystemManager::takeLock() const noexcept
{
    return m_mutex != nullptr && xSemaphoreTake(m_mutex, pdMS_TO_TICKS(2000U)) == pdTRUE;
}

void FileSystemManager::giveLock() const noexcept
{
    xSemaphoreGive(m_mutex);
}

} // namespace control_hub::drivers::filesystem
