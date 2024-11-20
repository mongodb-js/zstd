#ifndef MONGODB_ZSTD_COMPRESSION
#define MONGODB_ZSTD_COMPRESSION

#include <stdexcept>
#include <vector>

#include "compression_worker.h"
#include "zstd.h"

namespace mongodb_zstd {
std::vector<uint8_t> compress(const std::vector<uint8_t>& data, size_t compression_level);
std::vector<uint8_t> decompress(const std::vector<uint8_t>& data);
}  // namespace mongodb_zstd

#endif
