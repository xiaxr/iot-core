#include <memory>
#include <string>

#include <iot-core.h>

#include "network.h"

namespace {
const char* TAG = "wifi";
}

namespace xiaxr {

network_interface_t::network_interface_t() {}
network_interface_t::~network_interface_t() {}

namespace detail {
auto wifi_sta_start(const char* ssid, const char* passphrase = NULL,
                    int32_t channel = 0, const uint8_t* bssid = NULL,
                    bool connect = true) -> bool;
auto wifi_sta_disconnect() -> bool;
auto wifi_sta_is_connected() -> bool;
auto wifi_sta_ip_address() -> std::string;

auto sys_delay(unsigned long) -> void;

struct wifi_station_interface_t::data_t {
  data_t() : ssid{}, ip_address{}, connected{false} {}
  std::string ssid;
  std::string ip_address;
  bool        connected;
};

wifi_station_interface_t::wifi_station_interface_t() : _data{new data_t()} {}
wifi_station_interface_t::~wifi_station_interface_t() {
  if (_data->connected) {
    disconnect();
  }
}

auto wifi_station_interface_t::connect(
    const network_interface_settings_t* settings) -> bool {
  if (is_connected()) {
    LOG_TRACE(TAG, "Closing active station connection.")
    if (!disconnect()) {
      LOG_ERROR(TAG, "Failed to close existing connection.")
      return false;
    }
  }

  const wifi_station_interface_settings_t* s =
      reinterpret_cast<const wifi_station_interface_settings_t*>(settings);

  LOG_INFO(TAG, "Connecting to ", s->ssid, ".");

  if (!wifi_sta_start(s->ssid.c_str(),
                      s->password.empty() ? nullptr : s->password.c_str())) {
    LOG_ERROR(TAG, "Failed to start wifi sta connection.");
    return false;
  }

  while (!wifi_sta_is_connected()) {
    sys_delay(500);
    LOG_INFO(TAG, "Attempting to connect to ", s->ssid, ".");
  }

  LOG_INFO(TAG, "Connected.");

  _data->connected  = true;
  _data->ssid       = s->ssid;
  _data->ip_address = wifi_sta_ip_address();

  LOG_INFO(TAG, "IP address: ", _data->ip_address, ".");

  return true;
}

auto wifi_station_interface_t::ip_address() const -> const std::string& {
  return _data->ip_address;
}

auto wifi_station_interface_t::is_connected() const -> bool {
  return _data->connected;
}
auto wifi_station_interface_t::disconnect() -> bool {
  auto results      = wifi_sta_disconnect();
  _data->connected  = false;
  _data->ssid       = {};
  _data->ip_address = {};
  return results;
}
}  // namespace detail

struct network_manager_t::data_t {
  data_t() : _wifi_sta_iface{nullptr} {}
  std::unique_ptr<detail::wifi_station_interface_t> _wifi_sta_iface;
};

network_manager_t::network_manager_t() : _data{new data_t()} {}
network_manager_t::~network_manager_t() {}
auto network_manager_t::wifi_sta_connect(const std::string& ssid,
                                         const std::string& password) -> bool {
  _data->_wifi_sta_iface = std::make_unique<detail::wifi_station_interface_t>();
  detail::wifi_station_interface_settings_t settings = {ssid, password};
  auto results = _data->_wifi_sta_iface->connect(&settings);
  if (!results) {
    _data->_wifi_sta_iface = nullptr;
  }
  return results;
}

auto network_manager_t::is_internet_connected() const -> bool {
  return _data->_wifi_sta_iface != nullptr &&
         _data->_wifi_sta_iface->is_connected();
}

namespace detail {
network_manager_t network_manager;
}

auto connect_wifi_sta(const std::string& ssid, const std::string& password)
    -> bool {
  return detail::network_manager.wifi_sta_connect(ssid, password);
}

auto is_internet_connected() -> bool {
  return detail::network_manager.is_internet_connected();
}

}  // namespace xiaxr