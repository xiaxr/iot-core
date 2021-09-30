#include <string>
#include <string_view>
#include <memory>

#include <iot-core.h>

#include "logger.h"
#include "src/detail/logger_detail.h"

namespace xiaxr {
namespace {
constexpr log_level_t      default_log_level = log_level_t::info;
constexpr std::string_view default_log_tag{"app"};
}  // namespace

namespace detail {
struct serial_logger_t : logger_t {
 protected:
  virtual auto log_impl(const log_level_t level, const std::string& tag,
                        const std::string& message, const datetime_t& dt)
      -> void override {
    log_info_t info = {level, dt, tag, message};
    _out.post(info);
  }

 private:
  serial_logger_output_t _out;
};
}  // namespace detail

logger_t::logger_t() : _log_level{default_log_level} {}
logger_t::logger_t(log_level_t _level) : _log_level{_level} {}

auto logger_t::log(const std::string& message) -> void {
  log(_log_level, message);
}

auto logger_t::log(const log_level_t level, const std::string& message)
    -> void {
  log(level, default_log_tag.data(), message);
}

auto logger_t::log(const log_level_t level, const std::string& tag,
                   const std::string& message) -> void {
  log(level, tag, message, datetime_t::now());
}

auto logger_t::log(const log_level_t level, const std::string& tag,
                   const std::string& message, const datetime_t& dt) -> void {
  if ((int)level > (int)_log_level) {
    return;
  }
  log_impl(level, tag, message, dt);
}

#if defined(SERIAL_LOGGING)
detail::serial_logger_t serial_logger;
#endif

}  // namespace xiaxr
