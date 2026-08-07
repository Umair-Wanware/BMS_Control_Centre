#pragma once

#include <stdarg.h>

#include "esp_log.h"

/** @file logger.hpp @brief Small instance-based wrapper around ESP-IDF logging. */

namespace control_hub::utils {

/**
 * @brief Emits consistently tagged log records without introducing global state.
 */
class Logger final {
public:
    explicit constexpr Logger(const char* tag) noexcept : m_tag(tag) {}

    void verbose(const char* format, ...) const;
    void debug(const char* format, ...) const;
    void info(const char* format, ...) const;
    void warn(const char* format, ...) const;
    void error(const char* format, ...) const;

private:
    void write(esp_log_level_t level, const char* format, va_list arguments) const;

    const char* m_tag;
};

} // namespace control_hub::utils
