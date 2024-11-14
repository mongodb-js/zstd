#include "zstd.h"

#include <napi.h>

#include <string>
#include <vector>

#include "compress.h"
#include "compression_worker.h"
#include "decompress.h"
#include "napi_utils.h"

using namespace Napi;

Napi::Promise Compress(const Napi::CallbackInfo& info) {
    // Argument handling happens in JS
    if (info.Length() != 2) {
        std::string error_message = "Expected two arguments.";
        throw TypeError::New(info.Env(), error_message);
    }

    Uint8Array to_compress = Uint8ArrayFromValue(info[0], "buffer");
    std::vector<uint8_t> data = getBytesFromUint8Array(to_compress);

    size_t compression_level = (size_t)info[1].ToNumber().Int32Value();

    CompressionWorker* worker = new CompressionWorker(
        info.Env(),
        [data = std::move(data), compression_level] { return compress(data, compression_level); });

    worker->Queue();

    return worker->GetPromise();
}

Napi::Promise Decompress(const CallbackInfo& info) {
    // Argument handling happens in JS
    if (info.Length() != 1) {
        std::string error_message = "Expected one argument.";
        throw TypeError::New(info.Env(), error_message);
    }

    Napi::Uint8Array compressed_data = Uint8ArrayFromValue(info[0], "buffer");
    std::vector<uint8_t> data = getBytesFromUint8Array(compressed_data);
    CompressionWorker* worker =
        new CompressionWorker(info.Env(), [data = std::move(data)] { return decompress(data); });

    worker->Queue();

    return worker->GetPromise();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "compress"), Napi::Function::New(env, Compress));
    exports.Set(Napi::String::New(env, "decompress"), Napi::Function::New(env, Decompress));
    return exports;
}

NODE_API_MODULE(zstd, Init)
