#if defined ARDUINO_PLATFORM
#include <Arduino.h>

#include <memory.h>
#include <time.h>
#include <TZ.h>

#include <string>

#include "datetime.h"

#define NTP_CONNECTION_TIMEOUT (8 * 3600 * 2)
#define NTP_CONNECTION_DELAY 500

namespace xiaxr {
namespace detail {

auto initialize_datetime(const std::string &server_1,
                         const std::string &server_2,
                         const std::string &server_3) -> bool {
  configTime(TZ_Etc_UTC, server_1.c_str(), server_2.c_str(), server_3.c_str());

  time_t _now = time(nullptr);
  while (_now < NTP_CONNECTION_TIMEOUT) {
    delay(NTP_CONNECTION_DELAY);
    _now = time(nullptr);
  }
}

auto utc_now() -> datetime_t {
  struct tm info;
  memset(&info, 0, sizeof(info));

  time_t _now;
  time(&_now);
  localtime_r(&_now, &info);
  return {
      (uint8_t)info.tm_hour,      (uint8_t)info.tm_min,
      (uint8_t)info.tm_sec,       (uint8_t)info.tm_mday,
      (uint8_t)(info.tm_mon + 1), (uint16_t)(info.tm_year + 1900),
  };
}
}  // namespace detail
}  // namespace xiaxr

#undef NTP_CONNECTION_TIMEOUT
#undef NTP_CONNECTION_DELAY
#endif