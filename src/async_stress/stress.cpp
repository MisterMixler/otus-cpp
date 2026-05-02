#include "async.h"

#include <algorithm>
#include <random>
#include <string>
#include <thread>

namespace {

void worker(std::size_t bulk, const std::string &payload, int id) {
    void *const h = async::connect(bulk);
    std::mt19937 rng(static_cast<std::mt19937::result_type>(id + 17));
    std::uniform_int_distribution<std::size_t> chunk(1, 256);

    std::size_t offset = 0;
    while (offset < payload.size()) {
        const std::size_t n = std::min(chunk(rng), payload.size() - offset);
        async::receive(h, payload.data() + offset, n);
        offset += n;
    }

    async::disconnect(h);
}

std::string make_lines(std::size_t count, int ctx) {
    std::string s;
    s.reserve(count * 16);
    for (std::size_t i = 0; i < count; ++i) {
        s += "c";
        s += std::to_string(ctx);
        s += "_";
        s += std::to_string(i);
        s += '\n';
    }
    return s;
}

} // namespace

int main() {
    constexpr std::size_t bulk = 5;
    constexpr std::size_t lines_per_ctx = 2000;
    const std::string p0 = make_lines(lines_per_ctx, 0);
    const std::string p1 = make_lines(lines_per_ctx, 1);

    std::thread t0(worker, bulk, p0, 0);
    std::thread t1(worker, bulk, p1, 1);

    t0.join();
    t1.join();

    return 0;
}
