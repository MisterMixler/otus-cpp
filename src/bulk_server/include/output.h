#pragma once

#include "blocking_queue.h"
#include "observer.h"

#include <atomic>
#include <cstdint>

struct LogTask {
    Block block;
};

struct FileTask {
    Block block;
    std::uint64_t seq;
};

extern BlockingQueue<LogTask> g_log_queue;
extern BlockingQueue<FileTask> g_file_queue;
extern std::atomic<std::uint64_t> g_file_seq;

void log_worker();
void file_worker();

class OutputObserver final : public IObserver {
public:
    void onBlock(const Block &block) override;
};
