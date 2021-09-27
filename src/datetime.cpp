#include <string>
#include <stdio.h>

#include <iot-core.h>

namespace xiaxr {
namespace {
static bool     time_initialized = false;
constexpr char *ntp_server_1     = "time1.google.com";
constexpr char *ntp_server_2     = "time2.google.com";
constexpr char *ntp_server_3     = "time3.google.com";
}  // namespace

namespace detail {
auto initialize_datetime(const std::string &server_1,
                         const std::string &server_2,
                         const std::string &server_3) -> bool;
auto utc_now() -> datetime_t;
}  // namespace detail

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
  return initialize(ntp_server_1, ntp_server_2, ntp_server_3);
}

auto datetime_t::initialize(const std::string &server_1,
                            const std::string &server_2,
                            const std::string &server_3) -> bool {
  if (time_initialized) {
    return;
  }

  return detail::initialize_datetime(server_1, server_2, server_3);
}

auto datetime_t::now() -> datetime_t {
  if (!time_initialized) {
    initialize();
  }

  return detail::utc_now();
}

}  // namespace xiaxr

#undef NTP_CONNECTION_TIMEOUT
#undef NTP_CONNECTION_DELAY
