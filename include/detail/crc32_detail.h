#pragma once

#include <stdint.h>

namespace xiaxr {
namespace detail {
uint32_t crc32_le(uint32_t crc, const uint8_t* buf, uint32_t len);
}
}  // namespace xiaxr
