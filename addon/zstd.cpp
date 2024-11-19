#include "zstd.h"

#include <napi.h>

#include <vector>

#include "compression.h"
#include "compression_worker.h"

using namespace Napi;

namespace mongodb_zstd {
void Compress(const CallbackInfo& info) {
    // Argument handling happens in JS
    if (info.Length() != 3) {
        const char* error_message = "Expected three arguments.";
        throw TypeError::New(info.Env(), error_message);
    }

    Uint8Array to_compress = info[0].As<Uint8Array>();
    std::vector<uint8_t> data(to_compress.Data(), to_compress.Data() + to_compress.ElementLength());

    size_t compression_level = static_cast<size_t>(info[1].ToNumber().Int32Value());
    Function callback = info[2].As<Function>();

    CompressionWorker* worker =
        new CompressionWorker(callback, [data = std::move(data), compression_level] {
            return mongodb_zstd::compress(data, compression_level);
        });

    worker->Queue();
}

void Decompress(const CallbackInfo& info) {
    // Argument handling happens in JS
    if (info.Length() != 2) {
        const char* error_message = "Expected two argument.";
        throw TypeError::New(info.Env(), error_message);
    }

    Uint8Array compressed_data = info[0].As<Uint8Array>();
    std::vector<uint8_t> data(compressed_data.Data(),
                              compressed_data.Data() + compressed_data.ElementLength());
    Function callback = info[1].As<Function>();

    CompressionWorker* worker = new CompressionWorker(
        callback, [data = std::move(data)] { return mongodb_zstd::decompress(data); });

    worker->Queue();
}

Object Init(Env env, Object exports) {
    exports.Set(String::New(env, "compress"), Function::New(env, Compress));
    exports.Set(String::New(env, "decompress"), Function::New(env, Decompress));
    return exports;
}

NODE_API_MODULE(zstd, Init)

}  // namespace mongodb_zstd
