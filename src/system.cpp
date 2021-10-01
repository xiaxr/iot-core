#include <string>

#include <iot-core.h>

extern "C" {
#include <user_interface.h>
}

namespace xiaxr {
auto initialize_system(const std::string& ssid, const std::string& password)
    -> bool {
  if (!connect_wifi_sta(ssid, password)) {
    LOG_ERROR("sys", "Failed to connect to wifi network.");
    return false;
  }

  if (!connect_ntp()) {
    LOG_ERROR("sys", "Failed to connect to ntp servers.");
    return false;
  }
  if (!initialize_filesystem()) {
    LOG_ERROR("sys", "Failed to initialize filesystem.");
    return false;
  }
  return true;
}

}  // namespace xiaxr