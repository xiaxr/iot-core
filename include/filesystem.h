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

  auto initialize() -> bool;
  auto mounted() const -> bool;
  auto unmount() -> bool;

  auto read_file(const std::string& filename) -> std::string;
  auto write_file(const std::string& filename, const std::string& contents)
      -> size_t;
  auto append_file(const std::string& filename, const std::string& contents)
      -> size_t;

 private:
  struct data_t;
  std::unique_ptr<data_t> _data;
};

}  // namespace xiaxr