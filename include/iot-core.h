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

#define LOGGING_LEVEL LOG_LEVEL_INFO

#if defined(SERIAL_LOGGING)

namespace xiaxr {
extern logger_t serial_logger;
}  // namespace xiaxr

#define _LOG_LEVEL(level, tag, message)                                    \
  do {                                                                     \
    if (level == LOG_LEVEL_FATAL) {                                        \
      xiaxr::serial_logger.log(xiaxr::log_level_t::fatal, tag, message);   \
    } else if (level == LOG_LEVEL_ERROR) {                                 \
      xiaxr::serial_logger.log(xiaxr::log_level_t::error, tag, message);   \
    } else if (level == LOG_LEVEL_WARNING) {                               \
      xiaxr::serial_logger.log(xiaxr::log_level_t::warning, tag, message); \
    } else if (level == LOG_LEVEL_INFO) {                                  \
      xiaxr::serial_logger.log(xiaxr::log_level_t::info, tag, message);    \
    } else if (level == LOG_LEVEL_DEBUG) {                                 \
      xiaxr::serial_logger.log(xiaxr::log_level_t::debug, tag, message);   \
    } else if (level == LOG_LEVEL_TRACE) {                                 \
      xiaxr::serial_logger.log(xiaxr::log_level_t::trace, tag, message);   \
    }                                                                      \
  } while (0)

#define _LOG(level, tag, message)                                \
  do {                                                           \
    if (level <= LOGGING_LEVEL) _LOG_LEVEL(level, tag, message); \
  } while (0)

#define LOG_FATAL(tag, message) _LOG(LOG_LEVEL_FATAL, tag, message);
#define LOG_ERROR(tag, message) _LOG(LOG_LEVEL_ERROR, tag, message);
#define LOG_WARN(tag, message) _LOG(LOG_LEVEL_WARN, tag, message);
#define LOG_INFO(tag, message) _LOG(LOG_LEVEL_INFO, tag, message);
#define LOG_DEBUG(tag, message) _LOG(LOG_LEVEL_DEBUG, tag, message);
#define LOG_TRACE(tag, message) _LOG(LOG_LEVEL_TRACE, tag, message);
#else
#define LOG_FATAL(tag, message)
#define LOG_ERROR(tag, message)
#define LOG_WARN(tag, message)
#define LOG_INFO(tag, message)
#define LOG_DEBUG(tag, message)
#define LOG_TRACE(tag, message)
#endif

namespace xiaxr {}  // namespace xiaxr
