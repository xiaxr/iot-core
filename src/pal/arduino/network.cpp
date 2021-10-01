#if defined ARDUINO_PLATFORM
#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <string>

#include "network.h"

namespace xiaxr {
namespace {
auto convert_wifi_encryption(uint8_t value) -> wifi_encryption_t {
  switch (value) {
    case ENC_TYPE_WEP: return wifi_encryption_t::wep;
    case ENC_TYPE_TKIP: return wifi_encryption_t::wpa;
    case ENC_TYPE_CCMP: return wifi_encryption_t::wpa2;
    case ENC_TYPE_AUTO: return wifi_encryption_t::auto_;
    default: return wifi_encryption_t::none;
  }
}
}  // namespace
namespace detail {
auto wifi_set_sta_mode() -> void { WiFi.mode(WIFI_STA); }

auto wifi_sta_start(const char* ssid, const char* passphrase = NULL,
                    int32_t channel = 0, const uint8_t* bssid = NULL,
                    bool connect = true) -> bool {
  return WiFi.begin(ssid, passphrase, channel, bssid, connect) !=
         WL_CONNECT_FAILED;
}
auto wifi_sta_disconnect() -> bool { return WiFi.disconnect(); }
auto wifi_sta_is_connected() -> bool { return WiFi.isConnected(); }
auto wifi_sta_ip_address() -> std::string {
  return std::string(WiFi.localIP().toString().c_str());
}

auto wifi_scan_networks() -> size_t { return WiFi.scanNetworks(false, true); }
auto wifi_station_info(size_t index) -> wifi_station_t {
  return wifi_station_t{
      WiFi.SSID(index).c_str(),
      convert_wifi_encryption(WiFi.encryptionType(index)),
      WiFi.RSSI(index),
      WiFi.BSSIDstr(index).c_str(),
      WiFi.channel(index),
      WiFi.isHidden(index),
  };
}

}  // namespace detail
}  // namespace xiaxr

#endif
