#include "zstd.h"

#include <napi.h>

#include <string>
#include <vector>

#include "compression.h"
#include "compression_worker.h"
#include "napi_utils.h"

using namespace Napi;

void Compress(const Napi::CallbackInfo& info) {
    // Argument handling happens in JS
    if (info.Length() != 3) {
        std::string error_message = "Expected two arguments.";
        throw TypeError::New(info.Env(), error_message);
    }

    Uint8Array to_compress = Uint8ArrayFromValue(info[0], "buffer");
    std::vector<uint8_t> data = getBytesFromUint8Array(to_compress);

    size_t compression_level = (size_t)info[1].ToNumber().Int32Value();
    const Napi::Function& callback = info[2].As<Function>();

    CompressionWorker* worker =
        new CompressionWorker(callback, [data = std::move(data), compression_level] {
            return Compression::compress(data, compression_level);
        });

    worker->Queue();
}

void Decompress(const CallbackInfo& info) {
    // Argument handling happens in JS
    if (info.Length() != 2) {
        std::string error_message = "Expected one argument.";
        throw TypeError::New(info.Env(), error_message);
    }

    Napi::Uint8Array compressed_data = Uint8ArrayFromValue(info[0], "buffer");
    std::vector<uint8_t> data = getBytesFromUint8Array(compressed_data);
    const Napi::Function& callback = info[1].As<Function>();

    CompressionWorker* worker = new CompressionWorker(
        callback, [data = std::move(data)] { return Compression::decompress(data); });

    worker->Queue();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "compress"), Napi::Function::New(env, Compress));
    exports.Set(Napi::String::New(env, "decompress"), Napi::Function::New(env, Decompress));
    return exports;
}

NODE_API_MODULE(zstd, Init)
