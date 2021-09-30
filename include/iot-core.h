#pragma once

#include "datetime.h"
#include "filesystem.h"
#include "logger.h"

#define SERIAL_LOGGING

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_FATAL 1
#define LOG_LEVEL_ERROR 2
#define LOG_LEVEL_WARNING 3
#define LOG_LEVEL_INFO 4
#define LOG_LEVEL_DEBUG 5
#define LOG_LEVEL_TRACE 6

#define LOGGING_LEVEL 4

#if defined(SERIAL_LOGGING)
#define _LOG_LEVEL(level, tag, ...)                                \
  do {                                                             \
    if (level == LOG_LEVEL_FATAL) {                                \
      _xiaxr_log_v(xiaxr::log_level_t::fatal, tag, __VA_ARGS__);   \
    } else if (level == LOG_LEVEL_ERROR) {                         \
      _xiaxr_log_v(xiaxr::log_level_t::error, tag, __VA_ARGS__);   \
    } else if (level == LOG_LEVEL_WARNING) {                       \
      _xiaxr_log_v(xiaxr::log_level_t::warning, tag, __VA_ARGS__); \
    } else if (level == LOG_LEVEL_INFO) {                          \
      _xiaxr_log_v(xiaxr::log_level_t::info, tag, __VA_ARGS__);    \
    } else if (level == LOG_LEVEL_DEBUG) {                         \
      _xiaxr_log_v(xiaxr::log_level_t::debug, tag, __VA_ARGS__);   \
    } else if (level == LOG_LEVEL_TRACE) {                         \
      _xiaxr_log_v(xiaxr::log_level_t::trace, tag, __VA_ARGS__);   \
    }                                                              \
  } while (0)

#define _LOG(level, tag, ...)                                        \
  do {                                                               \
    if (level <= LOGGING_LEVEL) _LOG_LEVEL(level, tag, __VA_ARGS__); \
  } while (0)

#define LOG_FATAL(tag, ...) _LOG(LOG_LEVEL_FATAL, tag, __VA_ARGS__);
#define LOG_ERROR(tag, ...) _LOG(LOG_LEVEL_ERROR, tag, __VA_ARGS__);
#define LOG_WARN(tag, ...) _LOG(LOG_LEVEL_WARN, tag, __VA_ARGS__);
#define LOG_INFO(tag, ...) _LOG(LOG_LEVEL_INFO, tag, __VA_ARGS__);
#define LOG_DEBUG(tag, ...) _LOG(LOG_LEVEL_DEBUG, tag, __VA_ARGS__);
#define LOG_TRACE(tag, ...) _LOG(LOG_LEVEL_TRACE, tag, __VA_ARGS__);
#else
#define LOG_FATAL(tag, ...)
#define LOG_ERROR(tag, ...)
#define LOG_WARN(tag, ...)
#define LOG_INFO(tag, ...)
#define LOG_DEBUG(tag, ...)
#define LOG_TRACE(tag, ...)
#endif

namespace xiaxr {}  // namespace xiaxr
