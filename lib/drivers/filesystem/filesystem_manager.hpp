#pragma once

#include <stddef.h>
#include <stdint.h>

#include <string>
#include <vector>

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/** @file filesystem_manager.hpp @brief Thread-safe LittleFS file operations. */

namespace control_hub::drivers::filesystem {

/**
 * @brief Mounts and manages the LittleFS partition used by the Control Hub.
 */
class FileSystemManager final {
public:
    /** @brief LittleFS mount settings. */
    struct Config {
        const char* basePath = "/littlefs";
        const char* partitionLabel = "storage";
        bool formatIfMountFailed = true;
    };

    FileSystemManager();
    explicit FileSystemManager(Config config);
    ~FileSystemManager();

    FileSystemManager(const FileSystemManager&) = delete;
    FileSystemManager& operator=(const FileSystemManager&) = delete;

    /** Mounts the configured LittleFS partition. */
    esp_err_t initialize();

    /** Unmounts the partition and releases its synchronization resource. */
    esp_err_t deinitialize();

    /** Reads an entire relative file into memory. */
    esp_err_t readFile(const char* relativePath, std::vector<uint8_t>& contents) const;

    /** Reads a bounded byte range without loading the entire file. */
    esp_err_t readFileChunk(const char* relativePath, size_t offset, uint8_t* buffer,
                            size_t bufferCapacity, size_t& bytesRead) const;

    /** Atomically replaces a file's contents. */
    esp_err_t writeFile(const char* relativePath, const uint8_t* data, size_t length) const;

    /** Opaque handle for chunked file writes without loading the full payload into RAM. */
    struct StreamingWriteHandle {
        void* file = nullptr;
        std::string tempPath;
        std::string finalPath;
        bool active = false;
    };

    /** Opens a temporary file for chunked writing. */
    esp_err_t beginStreamingWrite(const char* relativePath,
                                    StreamingWriteHandle& handle) const;

    /** Appends one chunk to an open streaming write. */
    esp_err_t appendStreamingWrite(StreamingWriteHandle& handle, const uint8_t* data,
                                   size_t length) const;

    /** Renames the temporary file to its final path. */
    esp_err_t finishStreamingWrite(StreamingWriteHandle& handle) const;

    /** Discards an in-progress streaming write. */
    esp_err_t abortStreamingWrite(StreamingWriteHandle& handle) const;

    /** Removes a file when it exists. */
    esp_err_t removeFile(const char* relativePath) const;

    /** Returns a file's byte size. */
    esp_err_t getFileSize(const char* relativePath, size_t& size) const;

    /** Erases and recreates the mounted LittleFS partition. */
    esp_err_t format();

    /** Returns whether LittleFS is currently mounted. */
    bool isMounted() const noexcept;

    /** Returns the absolute mount path used for the partition. */
    const char* basePath() const noexcept;

private:
    bool isSafeRelativePath(const char* relativePath) const noexcept;
    std::string makeAbsolutePath(const char* relativePath) const;
    bool takeLock() const noexcept;
    void giveLock() const noexcept;

    Config m_config;
    bool m_mounted = false;
    mutable SemaphoreHandle_t m_mutex = nullptr;
};

} // namespace control_hub::drivers::filesystem
