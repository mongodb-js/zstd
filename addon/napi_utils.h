#include <napi.h>

using namespace Napi;

/**
 * @brief Given a T* source and a T* destination, copies count
 * elements from source into destination.
 */
template <typename T>
void copy_buffer_data(T* source, T* dest, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        dest[i] = source[i];
    }
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