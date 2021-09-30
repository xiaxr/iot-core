#pragma once

#include <string>
#include <optional>

namespace xiaxr {
struct datetime_t {
  datetime_t();
  datetime_t(uint8_t _hour, uint8_t _minute, uint8_t _second, uint8_t _day,
             uint8_t _month, uint16_t _year);
  ~datetime_t();

  auto to_string() const -> const std::string;

  static auto initialized() -> bool;

  static auto initialize() -> bool;
  static auto initialize(const std::string &server_1,
                         const std::string &server_2,
                         const std::string &server_3) -> bool;
  static auto now() -> datetime_t;
  static auto now_if_ready() -> std::optional<datetime_t>;

  uint8_t  hour;
  uint8_t  minute;
  uint8_t  second;
  uint8_t  day;
  uint8_t  month;
  uint16_t year;
};
}  // namespace xiaxr
