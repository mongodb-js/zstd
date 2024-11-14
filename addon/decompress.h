#include <vector>

#include "compression_worker.h"  // CompressionResult
#include "zstd.h"

CompressionResult decompress(const std::vector<uint8_t>& compressed) {
    std::vector<uint8_t> decompressed;

    using DCTX_Deleter = void (*)(ZSTD_DCtx*);

    std::unique_ptr<ZSTD_DCtx, DCTX_Deleter> decompression_context(ZSTD_createDCtx(),
                                                                   (DCTX_Deleter)ZSTD_freeDCtx);

    ZSTD_inBuffer input = {compressed.data(), compressed.size(), 0};

    while (input.pos < input.size) {
        std::vector<uint8_t> chunk(ZSTD_DStreamOutSize());
        ZSTD_outBuffer output = {chunk.data(), chunk.size(), 0};
        size_t const ret = ZSTD_decompressStream(decompression_context.get(), &output, &input);
        if (ZSTD_isError(ret)) {
            std::string error(ZSTD_getErrorName(ret));
            return CompressionResult::Error(error);
        }

        for (size_t i = 0; i < output.pos; ++i) {
            decompressed.push_back(chunk[i]);
        }
    }

    return CompressionResult::Ok(decompressed);
}
