#include <memory>

#include <iot-core.h>

#include "filesystem.h"
#include "detail/pal_detail.h"

namespace {
static const char* TAG = "fs";
}

namespace xiaxr {
struct file_t::data_t {
  data_t() {}
};

struct dir_t::data_t {
  data_t() {}
};

struct filesystem_t::data_t {
  data_t() : initialized{false} {}
  bool initialized;
};

file_t::file_t() : _data{new data_t()} {}
file_t::~file_t() {}

dir_t::dir_t() : _data{new data_t()} {}
dir_t::~dir_t() {}

filesystem_t::filesystem_t() : _data{new data_t()} {}
filesystem_t::~filesystem_t() {
  if (_data->initialized) {
    unmount();
  }
}

auto filesystem_t::initialize() -> bool {
  if (_data->initialized) {
    return true;
  }

  if (!detail::lfs_initialize()) {
    LOG_ERROR(TAG, "Failed to initialize filesystem.")
    _data->initialized = false;
    return false;
  }

  LOG_INFO(TAG, "Filesystem initialized.")
  _data->initialized = true;
  return true;
}

auto filesystem_t::mounted() const -> bool { return _data->initialized; }
auto filesystem_t::unmount() -> bool {
  auto results = detail::lfs_unmount();
  if (!results) {
    LOG_ERROR(TAG, "Failed to unmount filesystem.");
  }
  _data->initialized = false;
  return results;
}

auto filesystem_t::read_file(const std::string& filename) -> std::string {
  if (!_data->initialized) {
    return {};
  }
  return detail::lfs_read_file(filename);
}
auto filesystem_t::write_file(const std::string& filename,
                              const std::string& contents) -> size_t {
  if (!_data->initialized) {
    return 0;
  }
  return detail::lfs_write_file(filename, contents);
}
auto filesystem_t::append_file(const std::string& filename,
                               const std::string& contents) -> size_t {
  if (!_data->initialized) {
    return 0;
  }
  return detail::lfs_append_file(filename, contents);
}

namespace detail {
filesystem_t filesystem;
}

auto initialize_filesystem() -> bool { return detail::filesystem.initialize(); }

auto read_file(const std::string& filename) -> std::string {
  initialize_filesystem();
  return detail::filesystem.read_file(filename);
}

auto write_file(const std::string& filename, const std::string& contents)
    -> size_t {
  initialize_filesystem();
  return detail::filesystem.write_file(filename, contents);
}

auto append_file(const std::string& filename, const std::string& contents)
    -> size_t {
  initialize_filesystem();
  return detail::filesystem.append_file(filename, contents);
}

}  // namespace xiaxr