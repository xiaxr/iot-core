#pragma once

#include <memory>
#include <string>
#include <functional>

#include "datetime.h"

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

struct log_info_t {
  log_level_t level;
  datetime_t  datetime;
  std::string tag;
  std::string message;
};

using log_formatter_f = std::function<std::string(const log_info_t&)>;

struct log_formatter_t {
  log_formatter_t();
  log_formatter_t(log_formatter_f formatter);
  log_formatter_t(log_formatter_t&& other);
  ~log_formatter_t();

  auto format(const log_info_t& info) const -> std::string;

 private:
  struct data_t;
  std::unique_ptr<data_t> _data;
};

struct log_output_t;
using log_output_f =
    std::function<bool(const log_output_t& context, const log_info_t&)>;

struct log_output_t {
  log_output_t();
  log_output_t(log_output_f output, log_formatter_t&& formatter);
  ~log_output_t();

  auto format(const log_info_t& info) const -> std::string;
  auto post(const log_info_t& info) const -> bool;

 private:
  struct data_t;
  std::unique_ptr<data_t> _data;
};

struct logger_t {
  logger_t();
  logger_t(log_level_t level);
  ~logger_t();

  auto add_output(log_output_t&& value) -> void;

  auto log(const std::string& message) -> void;
  auto log(const log_level_t level, const std::string& message) -> void;
  auto log(const log_level_t level, const std::string& tag,
           const std::string& message) -> void;
  auto log(const log_level_t level, const std::string& tag,
           const std::string& message, const datetime_t& dt) -> void;

 private:
  struct data_t;
  std::unique_ptr<data_t> _data;
};

}  // namespace xiaxr