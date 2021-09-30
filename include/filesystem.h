#pragma once

#include <memory>

namespace xiaxr {

struct file_t {
  file_t();
  ~file_t();

 private:
  struct data_t;
  std::unique_ptr<data_t> _data;
};

struct dir_t {
  dir_t();
  ~dir_t();

 private:
  struct data_t;
  std::unique_ptr<data_t> _data;
};

struct filesystem_t {
  filesystem_t();
  ~filesystem_t();

 private:
  struct data_t;
  std::unique_ptr<data_t> _data;
};
}  // namespace xiaxr