#include <memory>
#include <string>
#include <vector>

#include <iot-core.h>

#include "network.h"
#include "detail/pal_detail.h"

namespace {
const char*             TAG              = "wifi";
constexpr unsigned long wifi_sta_timeout = 30 * 1000;
}  // namespace

namespace xiaxr {
network_interface_t::network_interface_t() {}
network_interface_t::~network_interface_t() {}
namespace detail {
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

  if (!detail::wifi_sta_connect(wifi_sta_timeout, s->ssid, s->password)) {
    LOG_ERROR(TAG, "Failed to connect to wifi network.");
    _data->connected  = false;
    _data->ssid       = {};
    _data->ip_address = {};
    return false;
  }

  _data->connected  = true;
  _data->ssid       = s->ssid;
  _data->ip_address = wifi_sta_ip_address();

  LOG_INFO(TAG, "Successfully connected to wifi network.");
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

auto network_manager_t::scan_wifi_networks() -> std::vector<wifi_station_t> {
  detail::wifi_set_sta_mode();
  if (_data->_wifi_sta_iface != nullptr) {
    LOG_DEBUG(TAG, "Disconnecting wifi to scan networks.")
    _data->_wifi_sta_iface->disconnect();
    detail::sys_delay(100);
    _data->_wifi_sta_iface = nullptr;
  }

  LOG_INFO(TAG, "Scanning networks.");
  auto network_count = detail::wifi_scan_networks();
  LOG_INFO(TAG, "Network scan complete.");

  std::vector<wifi_station_t> output;
  for (size_t i = 0; i < network_count; i++) {
    output.push_back(detail::wifi_station_info(i));
  }
  return output;
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

auto scan_wifi_networks() -> std::vector<wifi_station_t> {
  return detail::network_manager.scan_wifi_networks();
}

}  // namespace xiaxr