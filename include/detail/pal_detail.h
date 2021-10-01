#pragma once

#include <string>

#include "datetime.h"
#include "network.h"

namespace xiaxr {
namespace detail {
// ** datetime **
auto initialize_datetime(const unsigned long timeout_ms,
                         const std::string&  server_1,
                         const std::string&  server_2,
                         const std::string&  server_3) -> bool;
auto utc_now() -> datetime_t;

// ** wifi **
auto wifi_set_sta_mode() -> void;
auto wifi_sta_connect(const unsigned long timeout_ms, const std::string& ssid,
                      const std::string& passphrase, int32_t channel,
                      const std::string& bssid, bool connect) -> bool;
auto wifi_sta_connect(const unsigned long timeout_ms, const std::string& ssid,
                      const std::string& passphrase) -> bool;
auto wifi_sta_disconnect() -> bool;
auto wifi_sta_is_connected() -> bool;
auto wifi_sta_ip_address() -> std::string;

auto wifi_scan_networks() -> size_t;
auto wifi_station_info(size_t index) -> wifi_station_t;

// ** filesystem **
auto lfs_initialize() -> bool;
auto lfs_unmount() -> bool;
auto lfs_read_file(const std::string& filename) -> std::string;
auto lfs_write_file(const std::string& filename, const std::string& content)
    -> size_t;
auto lfs_append_file(const std::string& filename, const std::string& content)
    -> size_t;

// ** serial **
auto serial_print_line(const std::string& value) -> size_t;

// ** sys **
auto sys_delay(unsigned long v) -> void;
auto sys_yield() -> void;
}  // namespace detail
}  // namespace xiaxr
