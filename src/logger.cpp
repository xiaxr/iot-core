#include <string>
#include <string_view>
#include <memory>
#include <vector>

#include <iot-core.h>

#include "logger.h"

namespace xiaxr {
namespace detail {
auto serial_print_line(const std::string&) -> size_t;
}

namespace {
constexpr log_level_t      default_log_level = log_level_t::info;
constexpr std::string_view default_log_tag{"app"};

auto log_level_to_string(log_level_t level) -> std::string {
  switch (level) {
    case log_level_t::fatal: return "Fatal";
    case log_level_t::error: return "Error";
    case log_level_t::warning: return "Warn";
    case log_level_t::info: return "Info";
    case log_level_t::debug: return "Debug";
    case log_level_t::trace: return "Trace";
    case log_level_t::none:
    default: return "";
  }
}

auto default_log_formatter(const log_info_t& info) -> std::string {
  return "[" + log_level_to_string(info.level) + "] (" +
         info.datetime.to_string() + ") " + info.tag + ": " + info.message;
}

auto default_log_output(const log_output_t& context, const log_info_t& info)
    -> bool {
  auto txt = context.format(info);
  return detail::serial_print_line(txt) > 0;
}
}  // namespace

// **  Log Formatter
struct log_formatter_t::data_t {
  data_t(log_formatter_f _formatter) : formatter{_formatter} {}
  log_formatter_f formatter;
};

log_formatter_t::log_formatter_t() : _data{new data_t(default_log_formatter)} {}
log_formatter_t::log_formatter_t(log_formatter_f formatter)
    : _data{new data_t(formatter)} {}
log_formatter_t::log_formatter_t(log_formatter_t&& other) : _data(nullptr) {
  _data = std::move(other._data);
}
log_formatter_t::~log_formatter_t() {}
auto log_formatter_t::format(const log_info_t& info) const -> std::string {
  return _data->formatter(info);
}

// ** Log Output
struct log_output_t::data_t {
  data_t(log_output_f _output, log_formatter_t&& _formatter)
      : output{_output}, formatter(std::move(_formatter)) {}
  log_output_f    output;
  log_formatter_t formatter;
};

log_output_t::log_output_t()
    : _data{new data_t(default_log_output, log_formatter_t())} {}
log_output_t::log_output_t(log_output_f output, log_formatter_t&& formatter)
    : _data{new data_t(output, std::move(formatter))} {}
log_output_t::~log_output_t() {}

auto log_output_t::format(const log_info_t& info) const -> std::string {
  return _data->formatter.format(info);
}
auto log_output_t::post(const log_info_t& info) const -> bool {
  return _data->output(*this, info);
}

// ** Logger
struct logger_t::data_t {
  data_t(log_level_t _level) : level{_level}, output{} {}
  log_level_t               level;
  std::vector<log_output_t> output;
};

logger_t::logger_t() : _data{new data_t(default_log_level)} {
  add_output(std::move(log_output_t()));
}
logger_t::logger_t(log_level_t level) : _data(new data_t(level)) {}
logger_t::~logger_t() {}

auto logger_t::add_output(log_output_t&& value) -> void {
  _data->output.push_back(std::move(value));
}

auto logger_t::log(const std::string& message) -> void {
  log(_data->level, message);
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
  if ((int)level > (int)_data->level || _data->output.empty()) {
    return;
  }

  log_info_t info{level, dt, tag, message};

  for (auto const& o : _data->output) {
    o.post(info);
  }
}

#if defined(SERIAL_LOGGING)
logger_t serial_logger;
#endif

}  // namespace xiaxr