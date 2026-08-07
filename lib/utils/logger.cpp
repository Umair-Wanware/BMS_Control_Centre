#include "utils/logger.hpp"

namespace control_hub::utils {

void Logger::verbose(const char* format, ...) const
{
    va_list arguments;
    va_start(arguments, format);
    write(ESP_LOG_VERBOSE, format, arguments);
    va_end(arguments);
}

void Logger::debug(const char* format, ...) const
{
    va_list arguments;
    va_start(arguments, format);
    write(ESP_LOG_DEBUG, format, arguments);
    va_end(arguments);
}

void Logger::info(const char* format, ...) const
{
    va_list arguments;
    va_start(arguments, format);
    write(ESP_LOG_INFO, format, arguments);
    va_end(arguments);
}

void Logger::warn(const char* format, ...) const
{
    va_list arguments;
    va_start(arguments, format);
    write(ESP_LOG_WARN, format, arguments);
    va_end(arguments);
}

void Logger::error(const char* format, ...) const
{
    va_list arguments;
    va_start(arguments, format);
    write(ESP_LOG_ERROR, format, arguments);
    va_end(arguments);
}

void Logger::write(const esp_log_level_t level, const char* format, va_list arguments) const
{
    esp_log_writev(level, m_tag, format, arguments);
}

} // namespace control_hub::utils
