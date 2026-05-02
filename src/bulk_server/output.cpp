#include "output.h"

#include <fstream>
#include <iostream>
#include <string>

BlockingQueue<LogTask> g_log_queue;
BlockingQueue<FileTask> g_file_queue;
std::atomic<std::uint64_t> g_file_seq{0};

namespace {

void write_block_console(const Block &block) {
    std::cout << "bulk: ";
    for (std::size_t i = 0; i < block.commands.size(); ++i) {
        if (i > 0)
            std::cout << ", ";
        std::cout << block.commands[i];
    }
    std::cout << std::endl;
}

void write_block_file(const Block &block, std::uint64_t seq) {
    const std::string filename =
        "bulk" + std::to_string(block.timestamp) + "_" + std::to_string(seq) + ".log";
    std::ofstream file(filename);
    if (!file.is_open())
        return;
    file << "bulk: ";
    for (std::size_t i = 0; i < block.commands.size(); ++i) {
        if (i > 0)
            file << ", ";
        file << block.commands[i];
    }
    file << std::endl;
}

} // namespace

void log_worker() {
    LogTask task;
    while (g_log_queue.pop(task))
        write_block_console(task.block);
}

void file_worker() {
    FileTask task;
    while (g_file_queue.pop(task))
        write_block_file(task.block, task.seq);
}

void OutputObserver::onBlock(const Block &block) {
    const auto seq = g_file_seq.fetch_add(1, std::memory_order_relaxed);
    g_log_queue.push(LogTask{block});
    g_file_queue.push(FileTask{block, seq});
}
