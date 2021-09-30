#pragma once

#include <string>

namespace xiaxr {
enum class log_level_t {
  none    = 0,
  fatal   = 1,
  error   = 2,
  warning = 3,
  info    = 4,
  debug   = 5,
  trace   = 6,
};

struct logger_t {
 protected:
  logger_t();
  logger_t(log_level_t level);
  virtual ~logger_t() {}

 public:
  auto log(const std::string& message) -> void;
  auto log(const log_level_t level, const std::string& message) -> void;
  auto log(const log_level_t level, const std::string& tag,
           const std::string& message) -> void;
  auto log(const log_level_t level, const std::string& tag,
           const std::string& message, const datetime_t& dt) -> void;

 protected:
  virtual auto log_impl(const log_level_t level, const std::string& tag,
                        const std::string& message, const datetime_t& dt)
      -> void = 0;

  log_level_t _log_level;
};
}  // namespace xiaxr

auto _xiaxr_log(xiaxr::log_level_t level, const std::string& tag,
                const std::string& message) -> void;