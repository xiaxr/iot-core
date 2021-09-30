#pragma once

#include <memory>
#include <string>
#include <functional>

#include "datetime.h"
#include "logger.h"

namespace xiaxr {
namespace detail {
auto serial_print_line(const std::string&) -> size_t;
namespace {
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
}  // namespace
struct log_info_t {
  log_level_t level;
  datetime_t  datetime;
  std::string tag;
  std::string message;
};

template <typename F>
struct log_formatter_t {
  log_formatter_t() {}

  auto format(const log_info_t& info) -> std::string {
    return static_cast<F*>(this)->format_impl(info);
  }
};

struct default_log_formatter_t : log_formatter_t<default_log_formatter_t> {
  friend struct log_formatter_t<default_log_formatter_t>;

 private:
  auto format_impl(const log_info_t& info) -> std::string {
    return "[" + log_level_to_string(info.level) + "] (" +
           info.datetime.to_string() + ") " + info.tag + ": " + info.message;
  }
};

template <typename O>
struct log_output_t {
  log_output_t() {}

  auto post(const log_info_t& info) -> bool {
    return static_cast<O*>(this)->post_impl(info);
  }
};

struct default_log_output_t : log_output_t<default_log_output_t>,
                              default_log_formatter_t {
  friend struct log_output_t<default_log_output_t>;

 private:
  auto post_impl(const log_info_t& info) -> bool {
    return serial_print_line(format(info)) > 0;
  }
};

}  // namespace detail
using serial_logger_output_t = detail::default_log_output_t;
}  // namespace xiaxr
