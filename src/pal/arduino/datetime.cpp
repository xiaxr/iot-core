#if defined ARDUINO_PLATFORM
#include <Arduino.h>
#include <PolledTimeout.h>
#include <coredecls.h>

#include <memory.h>
#include <time.h>
#include <TZ.h>

#include <string>

#include "datetime.h"

#include <iot-core.h>

namespace xiaxr {
namespace detail {
namespace {
static bool time_set = false;

void time_is_set(bool from_sntp) { time_set = true; }
}  // namespace

auto initialize_datetime(const unsigned long timeout_ms,
                         const std::string&  server_1,
                         const std::string&  server_2,
                         const std::string&  server_3) -> bool {
  if (time_set) {
    return true;
  }

  esp8266::polledTimeout::oneShotMs ntp_timeout(timeout_ms);
  settimeofday_cb(time_is_set);
  configTime(TZ_Etc_UTC, server_1.c_str(), server_2.c_str(), server_3.c_str());
  yield();
  ntp_timeout.reset();
  while (!time_set && (!ntp_timeout)) {
    yield();
  }

  return time_set;
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

#endif