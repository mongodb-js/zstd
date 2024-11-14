#include <napi.h>

using namespace Napi;

Napi::String Compress(const Napi::CallbackInfo& info) {
    auto string = Napi::String::New(info.Env(), "compress()");
    return string;
}
Napi::String Decompress(const Napi::CallbackInfo& info) {
    auto string = Napi::String::New(info.Env(), "decompress()");
    return string;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "compress"), Napi::Function::New(env, Compress));
    exports.Set(Napi::String::New(env, "decompress"), Napi::Function::New(env, Decompress));
    return exports;
}

NODE_API_MODULE(zstd, Init)
