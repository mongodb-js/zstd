#ifndef COMPRESSION_WORKER_H
#define COMPRESSION_WORKER_H
#include <napi.h>

#include <optional>

using namespace Napi;

/**
 * @brief A class that represents the result of a compression operation.  Once the
 * MACOS_DEPLOYMENT_TARGET can be raised to 10.13 and use a c++17, we can remove this class and use
 * a std::optional<std::variant<std::vector<uint8_t>, std::string>>> instead.
 */
struct CompressionResult {
    CompressionResult(std::string error,
                      std::vector<uint8_t> result,
                      bool hasError,
                      bool hasResult,
                      bool initialized)
        : error(error),
          result(result),
          hasError(hasError),
          hasResult(hasResult),
          initialized(true) {}

   public:
    static CompressionResult Error(std::string error) {
        return CompressionResult(error, std::vector<uint8_t>(), true, false, true);
    }

    static CompressionResult Ok(std::vector<uint8_t> result) {
        return CompressionResult(std::string(""), result, false, true, true);
    }

    static CompressionResult Empty() {
        return CompressionResult(std::string(""), std::vector<uint8_t>(), false, false, false);
    }

    std::string error;
    std::vector<uint8_t> result;

    bool hasError;
    bool hasResult;
    bool initialized;
};

/**
 * @brief An asynchronous Napi::Worker that can be with any function that produces
 * CompressionResults.
 * */
class CompressionWorker : public Napi::AsyncWorker {
   public:
    CompressionWorker(const Napi::Env& env, std::function<CompressionResult()> worker)
        : Napi::AsyncWorker{env, "Worker"},
          m_deferred{env},
          worker(worker),
          result(CompressionResult::Empty()) {}

    Napi::Promise GetPromise() {
        return m_deferred.Promise();
    }

   protected:
    void Execute() {
        result = worker();
    }

    void OnOK() {
        if (!result.initialized) {
            m_deferred.Reject(Napi::Error::New(Env(),
                                               "zstd runtime error - async worker finished without "
                                               "a compression or decompression result.")
                                  .Value());
        } else if (result.hasError) {
            m_deferred.Reject(Napi::Error::New(Env(), result.error).Value());
        } else if (result.hasResult) {
            Buffer<uint8_t> output =
                Buffer<uint8_t>::Copy(m_deferred.Env(), result.result.data(), result.result.size());

            m_deferred.Resolve(output);
        } else {
            m_deferred.Reject(Napi::Error::New(Env(),
                                               "zstd runtime error - async worker finished without "
                                               "a compression or decompression result.")
                                  .Value());
        }
    }

    void OnError(const Napi::Error& err) {
        m_deferred.Reject(err.Value());
    }

   private:
    Napi::Promise::Deferred m_deferred;
    std::function<CompressionResult()> worker;
    CompressionResult result;
};

#endif
