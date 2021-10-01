#include <string>

#include <memory>
#include <optional>

#include <iot-core.h>

#include "logger.h"
#include "detail/logger_detail.h"

namespace xiaxr {
namespace {
static const char*    default_tag       = "app";
constexpr log_level_t default_log_level = log_level_t::info;
}  // namespace

namespace detail {
struct serial_logger_t : logger_t {
 protected:
  virtual auto log_impl(const log_level_t level, const std::string& tag,
                        const std::string&               message,
                        const std::optional<datetime_t>& dt) -> void override {
    log_info_t info = {level, dt, tag, message};
    _out.post(info);
  }

 private:
  serial_logger_output_t _out;
};

static serial_logger_t default_logger;

}  // namespace detail

logger_t::logger_t() : _log_level{default_log_level} {}
logger_t::logger_t(log_level_t _level) : _log_level{_level} {}

auto logger_t::log(const std::string& message) -> void {
  log(_log_level, message);
}

auto logger_t::log(const log_level_t level, const std::string& message)
    -> void {
  log(level, default_tag, message);
}

auto logger_t::log(const log_level_t level, const std::string& tag,
                   const std::string& message) -> void {
  log(level, tag, message, datetime_t::now_if_ready());
}

auto logger_t::log(const log_level_t level, const std::string& tag,
                   const std::string&               message,
                   const std::optional<datetime_t>& dt) -> void {
  if ((int)level > (int)_log_level) {
    return;
  }
  log_impl(level, tag, message, dt);
}
}  // namespace xiaxr

auto _xiaxr_log(xiaxr::log_level_t level, const std::string& tag,
                const std::string& message) -> void {
  xiaxr::detail::default_logger.log(level, tag, message);
}
