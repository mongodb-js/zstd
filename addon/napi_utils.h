#include <napi.h>

using namespace Napi;

std::vector<uint8_t> getBytesFromUint8Array(const Uint8Array& source) {
    const uint8_t* input_data = source.Data();
    size_t total = source.ElementLength();
    std::vector<uint8_t> data(total);

    std::copy(input_data, input_data + total, data.data());

    return data;
}

/**
 * @brief Given an Napi;:Value, this function returns the value as a Uint8Array, if the
 * Value is a Uint8Array. Otherwise, this function throws.
 *
 * @param v - An Napi::Value
 * @param argument_name - the name of the value, to use when constructing an error message.
 * @return Napi::Uint8Array
 */
Uint8Array Uint8ArrayFromValue(Value v, std::string argument_name) {
    if (!v.IsTypedArray() || v.As<TypedArray>().TypedArrayType() != napi_uint8_array) {
        std::string error_message = "Parameter `" + argument_name + "` must be a Uint8Array.";
        throw TypeError::New(v.Env(), error_message);
    }

    return v.As<Uint8Array>();
}
