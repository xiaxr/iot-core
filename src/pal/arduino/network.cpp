#if defined ARDUINO_PLATFORM
#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <string>

namespace xiaxr {
namespace detail {
auto wifi_sta_start(const char* ssid, const char* passphrase = NULL,
                    int32_t channel = 0, const uint8_t* bssid = NULL,
                    bool connect = true) -> bool {
  return WiFi.begin(ssid, passphrase, channel, bssid, connect) !=
         WL_CONNECT_FAILED;
}
auto wifi_sta_disconnect() -> bool { return WiFi.disconnect(); }
auto wifi_sta_is_connected() -> bool { return WiFi.isConnected(); }
auto wifi_sta_ip_address() -> std::string {
  return std::string(WiFi.localIP().toString());
}
}  // namespace detail
}  // namespace xiaxr

#endif
