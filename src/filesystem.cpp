#include <memory>

#include <iot-core.h>

#include "filesystem.h"

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
filesystem_t::~filesystem_t() {}

}  // namespace xiaxr