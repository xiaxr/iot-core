#if defined ARDUINO_PLATFORM
#include <Arduino.h>
#include <LittleFS.h>

#include <string>

#define FILESYSTEM LittleFS

namespace xiaxr {
namespace detail {
auto lfs_initialize() -> bool { return FILESYSTEM.begin(); }
auto lfs_unmount() -> bool { return FILESYSTEM.end(); }

auto lfs_read_file(const std::string& filename) -> std::string {
  auto file = FILESYSTEM.open(filename.c_str(), "r");

  if (!file) {
    return {};
  }

  auto output = std::string(file.readString().c_str());
  file.close();
  return output;
}

auto lfs_write_file(const std::string& filename, const std::string& content)
    -> size_t {
  auto file = FILESYSTEM.open(filename.c_str(), "w");
  if (!file) {
    return 0;
  }

  auto wrote = file.print(content.c_str());
  file.close();
  return wrote;
}

auto lfs_append_file(const std::string& filename, const std::string& content)
    -> size_t {
  auto file = FILESYSTEM.open(filename.c_str(), "a");
  if (!file) {
    return 0;
  }

  auto wrote = file.print(content.c_str());
  file.close();
  return wrote;
}
}  // namespace detail
}  // namespace xiaxr

#endif