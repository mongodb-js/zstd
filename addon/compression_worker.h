#ifndef COMPRESSION_WORKER_H
#define COMPRESSION_WORKER_H
#include <napi.h>

#include <optional>
#include <variant>

using namespace Napi;

using CompressionResult = std::variant<std::vector<uint8_t>, std::string>;

// Implementation of the Overload pattern:
// https://www.cppstories.com/2019/02/2lines3featuresoverload.html/
template <class... Ts>
struct overload : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overload(Ts...) -> overload<Ts...>;

/**
 * @brief An asynchronous Napi::Worker that can be with any function that produces
 * CompressionResults.
 * */
class CompressionWorker : public Napi::AsyncWorker {
   public:
    CompressionWorker(const Napi::Env& env, std::function<CompressionResult()> worker)
        : Napi::AsyncWorker{env, "Worker"}, m_deferred{env}, m_worker(worker), m_result{} {}

    Napi::Promise GetPromise() {
        return m_deferred.Promise();
    }

   protected:
    void Execute() {
        m_result = m_worker();
    }

    void OnOK() {
        if (!m_result.has_value()) {
            m_deferred.Reject(Napi::Error::New(Env(),
                                               "zstd runtime error - async worker finished without "
                                               "a compression or decompression result.")
                                  .Value());
            return;
        }

        auto result_visitor = overload{
            [m_deferred = this->m_deferred](std::string error_message) {
                auto error = Napi::Error::New(m_deferred.Env(), error_message);
                m_deferred.Reject(error.Value());
            },
            [m_deferred = this->m_deferred](std::vector<uint8_t> result) {
                Buffer<uint8_t> output =
                    Buffer<uint8_t>::Copy(m_deferred.Env(), result.data(), result.size());

                m_deferred.Resolve(output);
            },
        };
        std::visit(result_visitor, *m_result);
    }

    void OnError(const Napi::Error& err) {
        m_deferred.Reject(err.Value());
    }

   private:
    Napi::Promise::Deferred m_deferred;
    std::function<CompressionResult()> m_worker;
    std::optional<CompressionResult> m_result;
};

#endif
