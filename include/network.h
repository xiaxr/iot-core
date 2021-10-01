#pragma once

#include <memory>
#include <string>

namespace xiaxr {

struct network_interface_settings_t {};
struct network_interface_t {
  network_interface_t();
  virtual ~network_interface_t();

  virtual auto connect(const network_interface_settings_t* settings)
      -> bool                                           = 0;
  virtual auto ip_address() const -> const std::string& = 0;
  virtual auto is_connected() const -> bool             = 0;
  virtual auto disconnect() -> bool                     = 0;
};

namespace detail {
struct wifi_station_interface_settings_t : network_interface_settings_t {
  wifi_station_interface_settings_t(const std::string& _ssid,
                                    const std::string& _password)
      : ssid{_ssid}, password{_password} {}
  std::string ssid;
  std::string password;
};

struct wifi_station_interface_t : network_interface_t {
  wifi_station_interface_t();
  virtual ~wifi_station_interface_t();

  virtual auto connect(const network_interface_settings_t* settings)
      -> bool override;
  virtual auto ip_address() const -> const std::string& override;
  virtual auto is_connected() const -> bool override;
  virtual auto disconnect() -> bool override;

 private:
  struct data_t;
  std::unique_ptr<data_t> _data;
};
}  // namespace detail

struct network_manager_t {
  network_manager_t();
  ~network_manager_t();

  auto wifi_sta_connect(const std::string& ssid, const std::string& password)
      -> bool;

 private:
  struct data_t;
  std::unique_ptr<data_t> _data;
};
}  // namespace xiaxr