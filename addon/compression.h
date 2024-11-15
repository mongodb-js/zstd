#ifndef MONGODB_ZSTD_COMPRESSION
#define MONGODB_ZSTD_COMPRESSION

#include <vector>

#include "compression_worker.h"
#include "zstd.h"

namespace Compression {
CompressionResult compress(const std::vector<uint8_t>& data, size_t compression_level);
CompressionResult decompress(const std::vector<uint8_t>& data);
}  // namespace Compression

#endif
