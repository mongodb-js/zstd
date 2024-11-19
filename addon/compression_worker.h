#ifndef MONGODB_ZSTD_COMPRESSION_WORKER_H
#define MONGODB_ZSTD_COMPRESSION_WORKER_H
#include <napi.h>

#include <optional>
#include <variant>

using namespace Napi;

namespace mongodb_zstd {
/**
 * @brief An asynchronous Napi::Worker that can be with any function that produces
 * CompressionResults.
 * */
class CompressionWorker final : public AsyncWorker {
   public:
    CompressionWorker(const Function& callback, std::function<std::vector<uint8_t>()> worker)
        : AsyncWorker{callback, "compression worker"}, m_worker(worker), m_result{} {}

   protected:
    void Execute() final {
        m_result = m_worker();
    }

    void OnOK() final {
        if (!m_result.has_value()) {
            Callback().Call({Error::New(Env(),
                                        "zstd runtime  - async worker finished without "
                                        "a compression or decompression result.")
                                 .Value()});
            return;
        }

        const std::vector<uint8_t>& data = m_result.value();
        Buffer result = Buffer<uint8_t>::Copy(Env(), data.data(), data.size());

        Callback().Call({Env().Undefined(), result});
    }

   private:
    std::function<std::vector<uint8_t>()> m_worker;
    std::optional<std::vector<uint8_t>> m_result;
};

}  // namespace mongodb_zstd
#endif
