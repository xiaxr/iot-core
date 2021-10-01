#if defined ARDUINO_PLATFORM
#include <Arduino.h>

namespace xiaxr {
namespace detail {
auto sys_delay(unsigned long v) -> void { delay(v); }
}  // namespace detail
}  // namespace xiaxr
#endif