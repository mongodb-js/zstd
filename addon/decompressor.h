#include <napi.h>

#include <vector>

#include "zstd.h"

using namespace Napi;

struct Decompressor {
    std::vector<uint8_t> data;
    size_t buffer_size;

    Decompressor(std::vector<uint8_t> data, size_t buffer_size)
        : data(data), buffer_size(buffer_size) {}

    CompressionResult operator()() {
        std::vector<uint8_t> decompressed(buffer_size);

        size_t _result =
            ZSTD_decompress(decompressed.data(), decompressed.size(), data.data(), data.size());

        if (ZSTD_isError(_result)) {
            std::string error(ZSTD_getErrorName(_result));
            return CompressionResult::Error(error);
        }

        decompressed.resize(_result);

        return CompressionResult::Ok(decompressed);
    }

    static Decompressor fromUint8Array(const Uint8Array& compressed_data) {
        const uint8_t* input_data = compressed_data.Data();
        size_t total = compressed_data.ElementLength();

        std::vector<uint8_t> data(total);
        std::copy(input_data, input_data + total, data.data());

        return Decompressor(data, total * 1000);
    }
};
