#ifndef MONGODB_ZSTD_COMPRESSION
#define MONGODB_ZSTD_COMPRESSION

#include <exception>
#include <vector>

#include "compression_worker.h"
#include "zstd.h"

namespace Compression {
std::vector<uint8_t> compress(const std::vector<uint8_t>& data, size_t compression_level);
std::vector<uint8_t> decompress(const std::vector<uint8_t>& data);
}  // namespace Compression

#endif
