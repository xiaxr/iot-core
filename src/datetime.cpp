#include <string>
#include <string_view>
#include <optional>

#include <stdio.h>

#include <iot-core.h>

#include "datetime.h"
#include "detail/pal_detail.h"

namespace xiaxr {
namespace {
const char *               TAG              = "datetime";
static bool                time_initialized = false;
constexpr std::string_view ntp_server_1{"time1.google.com"};
constexpr std::string_view ntp_server_2{"time2.google.com"};
constexpr std::string_view ntp_server_3{"time3.google.com"};
}  // namespace

datetime_t::datetime_t()
    : hour{0}, minute{0}, second{0}, day{0}, month{0}, year{0} {}
datetime_t::datetime_t(uint8_t _hour, uint8_t _minute, uint8_t _second,
                       uint8_t _day, uint8_t _month, uint16_t _year)
    : hour{_hour},
      minute{_minute},
      second{_second},
      day{_day},
      month{_month},
      year{_year} {}

datetime_t::~datetime_t() {}

auto datetime_t::to_string() const -> const std::string {
  char output[27];
  snprintf(output, 27, "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour,
           minute, second);
  return std::string(output);
}

auto datetime_t::initialize() -> bool {
  return initialize(ntp_server_1.data(), ntp_server_2.data(),
                    ntp_server_3.data());
}

auto datetime_t::initialize(const std::string &server_1,
                            const std::string &server_2,
                            const std::string &server_3) -> bool {
  if (time_initialized) {
    return true;
  }

  time_initialized = detail::initialize_datetime(server_1, server_2, server_3);
  return time_initialized;
}

auto datetime_t::initialized() -> bool { return time_initialized; }

auto datetime_t::now() -> datetime_t {
  if (!time_initialized) {
    initialize();
  }

  return detail::utc_now();
}

auto datetime_t::now_if_ready() -> std::optional<datetime_t> {
  if (!time_initialized) {
    return std::nullopt;
  }

  return detail::utc_now();
}

auto connect_ntp() -> bool {
  if (time_initialized) {
    LOG_WARN(TAG, "Datetime has already been initialized.")
    return true;
  }

  if (!is_internet_connected()) {
    LOG_WARN(TAG,
             "Cannot connect to NTP servers without an internet connection.");
    return false;
  }

  LOG_INFO(TAG, "Initalizing datetime.");

  if (!datetime_t::initialize()) {
    LOG_ERROR(TAG, "Could not initialize datetime.")
    return false;
  }

  LOG_INFO(TAG, "Datetime initialized: ");
  return true;
}

}  // namespace xiaxr
