#include "zstd.h"

#include <napi.h>

#include <string>
#include <vector>

#include "compression_worker.h"
#include "compressor.h"
#include "decompressor.h"
#include "napi_utils.h"

using namespace Napi;

Napi::Promise Compress(const Napi::CallbackInfo& info) {
    // Argument handling happens in JS
    if (info.Length() != 2) {
        std::string error_message = "Expected two arguments.";
        throw TypeError::New(info.Env(), error_message);
    }

    Uint8Array to_compress = Uint8ArrayFromValue(info[0], "buffer");
    size_t compression_level = (size_t)info[1].ToNumber().Int32Value();

    Compressor compressor = Compressor::fromUint8Array(to_compress, compression_level);
    Worker<Compressor>* worker = new Worker<Compressor>(info.Env(), std::move(compressor));

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
    Decompressor decompressor = Decompressor::fromUint8Array(compressed_data);
    Worker<Decompressor>* worker = new Worker<Decompressor>(info.Env(), decompressor);

    worker->Queue();

    return worker->GetPromise();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "compress"), Napi::Function::New(env, Compress));
    exports.Set(Napi::String::New(env, "decompress"), Napi::Function::New(env, Decompress));
    return exports;
}

NODE_API_MODULE(zstd, Init)
