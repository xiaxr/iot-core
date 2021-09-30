#if defined ARDUINO_PLATFORM
#include <Arduino.h>
#include <string>

namespace xiaxr {
namespace detail {
namespace {
static bool serial_init = false;

void initialize_serial() {
  if (!serial_init) {
    Serial.begin(74880);
    serial_init = true;
  }
}

}  // namespace

auto serial_print_line(const std::string& value) -> size_t {
  if (!serial_init) {
    initialize_serial();
  }
  return Serial.println(value.c_str());
}
}  // namespace detail
}  // namespace xiaxr

#endif