#include "compression.h"

std::vector<uint8_t> mongodb_zstd::compress(const std::vector<uint8_t>& data,
                                            size_t compression_level) {
    size_t output_buffer_size = ZSTD_compressBound(data.size());
    std::vector<uint8_t> output(output_buffer_size);

    size_t result_code =
        ZSTD_compress(output.data(), output.size(), data.data(), data.size(), compression_level);

    if (ZSTD_isError(result_code)) {
        throw std::runtime_error(ZSTD_getErrorName(result_code));
    }

    output.resize(result_code);

    return output;
}

std::vector<uint8_t> mongodb_zstd::decompress(const std::vector<uint8_t>& compressed) {
    std::vector<uint8_t> decompressed;

    using DCTX_Deleter = void (*)(ZSTD_DCtx*);

    std::unique_ptr<ZSTD_DCtx, DCTX_Deleter> decompression_context(
        ZSTD_createDCtx(), [](ZSTD_DCtx* ctx) { ZSTD_freeDCtx(ctx); });

    ZSTD_inBuffer input = {compressed.data(), compressed.size(), 0};
    std::vector<uint8_t> output_buffer(ZSTD_DStreamOutSize());
    ZSTD_outBuffer output = {output_buffer.data(), output_buffer.size(), 0};

    //   Source: https://facebook.github.io/zstd/zstd_manual.html#Chapter9
    //
    //   Use ZSTD_decompressStream() repetitively to consume your input.
    //   The function will update both `pos` fields.
    //   If `input.pos < input.size`, some input has not been consumed.
    //   It's up to the caller to present again remaining data.
    //   The function tries to flush all data decoded immediately, respecting output buffer size.
    //   If `output.pos < output.size`, decoder has flushed everything it could.
    //   But if `output.pos == output.size`, there might be some data left within internal buffers.,
    //   In which case, call ZSTD_decompressStream() again to flush whatever remains in the buffer.
    //   Note : with no additional input provided, amount of data flushed is necessarily <=
    //   ZSTD_BLOCKSIZE_MAX.
    //  @return : 0 when a frame is completely decoded and fully flushed,
    //         or an error code, which can be tested using ZSTD_isError(),
    //         or any other value > 0, which means there is still some decoding or flushing to do to
    //         complete current frame :
    //                                 the return value is a suggested next input size (just a hint
    //                                 for better latency) that will never request more than the
    //                                 remaining frame size.
    auto inputRemains = [](const ZSTD_inBuffer& input) { return input.pos < input.size; };
    auto isOutputBufferFlushed = [](const ZSTD_outBuffer& output) {
        return output.pos < output.size;
    };

    while (inputRemains(input) || !isOutputBufferFlushed(output)) {
        size_t const ret = ZSTD_decompressStream(decompression_context.get(), &output, &input);
        if (ZSTD_isError(ret)) {
            throw std::runtime_error(ZSTD_getErrorName(ret));
        }

        size_t decompressed_size = decompressed.size();
        decompressed.resize(decompressed_size + output.pos);
        std::copy(output_buffer.data(),
                  output_buffer.data() + output.pos,
                  decompressed.data() + decompressed_size);

        // move the position back go 0, to indicate that we are ready for more data
        output.pos = 0;
    }

    return decompressed;
}
