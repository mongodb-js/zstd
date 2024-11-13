
#include <napi.h>

#include <vector>

#include "zstd.h"

using namespace Napi;

struct Compressor {
    std::vector<uint8_t> data;
    size_t compression_level;

    Compressor(std::vector<uint8_t> data, size_t compression_level)
        : data(data), compression_level(compression_level) {}

    CompressionResult operator()() {
        size_t output_buffer_size = ZSTD_compressBound(data.size());
        std::vector<uint8_t> output(output_buffer_size);

        size_t result_code = ZSTD_compress(
            output.data(), output.size(), data.data(), data.size(), compression_level);

        if (ZSTD_isError(result_code)) {
            std::string error(ZSTD_getErrorName(result_code));
            return CompressionResult::Error(error);
        }

        output.resize(result_code);

        return CompressionResult::Ok(output);
    }

    static Compressor fromUint8Array(const Uint8Array& to_compress, size_t compression_level) {
        const uint8_t* input_data = to_compress.Data();
        size_t total = to_compress.ElementLength();

        std::vector<uint8_t> data(to_compress.ElementLength());

        std::copy(input_data, input_data + total, data.data());

        return Compressor(std::move(data), compression_level);
    }
};
