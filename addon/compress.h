#include <vector>

#include "compression_worker.h"  // CompressionResult
#include "zstd.h"

CompressionResult compress(const std::vector<uint8_t> data, size_t compression_level) {
    size_t output_buffer_size = ZSTD_compressBound(data.size());
    std::vector<uint8_t> output(output_buffer_size);

    size_t result_code =
        ZSTD_compress(output.data(), output.size(), data.data(), data.size(), compression_level);

    if (ZSTD_isError(result_code)) {
        std::string error(ZSTD_getErrorName(result_code));
        return CompressionResult::Error(error);
    }

    output.resize(result_code);

    return CompressionResult::Ok(output);
}
