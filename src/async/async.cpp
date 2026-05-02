#include "async.h"

#include "command_processor.h"
#include "observer.h"

#include <atomic>
#include <condition_variable>
#include <deque>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace {

class AsyncRuntime;

struct LogTask {
    Block block;
};

struct FileTask {
    Block block;
    std::uint64_t seq;
};

template <typename T>
class BlockingQueue {
public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push_back(std::move(value));
        cv_.notify_one();
    }

    bool pop(T &out) {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { return !queue_.empty() || shutdown_; });
        if (queue_.empty()) {
            return false;
        }
        out = std::move(queue_.front());
        queue_.pop_front();
        return true;
    }

    void shutdown() {
        std::lock_guard<std::mutex> lock(mutex_);
        shutdown_ = true;
        cv_.notify_all();
    }

private:
    std::deque<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool shutdown_{false};
};

void write_block_console(const Block &block) {
    std::cout << "bulk: ";
    for (std::size_t i = 0; i < block.commands.size(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << block.commands[i];
    }
    std::cout << std::endl;
}

void write_block_file(const Block &block, std::uint64_t seq) {
    const std::string filename =
        "bulk" + std::to_string(block.timestamp) + "_" + std::to_string(seq) + ".log";
    std::ofstream file(filename);
    if (!file.is_open()) {
        return;
    }
    file << "bulk: ";
    for (std::size_t i = 0; i < block.commands.size(); ++i) {
        if (i > 0) {
            file << ", ";
        }
        file << block.commands[i];
    }
    file << std::endl;
}

class QueueingObserver final : public IObserver {
public:
    explicit QueueingObserver(AsyncRuntime *runtime) : runtime_(runtime) {}

    void onBlock(const Block &block) override;

private:
    AsyncRuntime *runtime_;
};

class AsyncRuntime {
public:
    static AsyncRuntime &instance() {
        static AsyncRuntime inst;
        return inst;
    }

    std::shared_ptr<QueueingObserver> observer() {
        ensure_started();
        return observer_;
    }

    void post_block(const Block &block) {
        const std::uint64_t seq = file_seq_.fetch_add(1, std::memory_order_relaxed);
        log_queue_.push(LogTask{block});
        file_queue_.push(FileTask{block, seq});
    }

    ~AsyncRuntime() { shutdown(); }

    AsyncRuntime(const AsyncRuntime &) = delete;
    AsyncRuntime &operator=(const AsyncRuntime &) = delete;

private:
    AsyncRuntime() = default;

    void ensure_started() {
        std::lock_guard<std::mutex> lock(start_mutex_);
        if (started_) {
            return;
        }
        observer_ = std::make_shared<QueueingObserver>(this);
        log_thread_ = std::thread([this] { log_worker(); });
        file_thread_1_ = std::thread([this] { file_worker(); });
        file_thread_2_ = std::thread([this] { file_worker(); });
        started_ = true;
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(start_mutex_);
            if (!started_) {
                return;
            }
        }
        log_queue_.shutdown();
        file_queue_.shutdown();
        if (log_thread_.joinable()) {
            log_thread_.join();
        }
        if (file_thread_1_.joinable()) {
            file_thread_1_.join();
        }
        if (file_thread_2_.joinable()) {
            file_thread_2_.join();
        }
    }

    void log_worker() {
        LogTask task;
        while (log_queue_.pop(task)) {
            write_block_console(task.block);
        }
    }

    void file_worker() {
        FileTask task;
        while (file_queue_.pop(task)) {
            write_block_file(task.block, task.seq);
        }
    }

    BlockingQueue<LogTask> log_queue_;
    BlockingQueue<FileTask> file_queue_;
    std::atomic<std::uint64_t> file_seq_{0};

    std::mutex start_mutex_;
    bool started_{false};
    std::shared_ptr<QueueingObserver> observer_;

    std::thread log_thread_;
    std::thread file_thread_1_;
    std::thread file_thread_2_;
};

void QueueingObserver::onBlock(const Block &block) { runtime_->post_block(block); }

struct AsyncContext {
    explicit AsyncContext(std::size_t bulk, std::shared_ptr<QueueingObserver> obs)
        : processor(bulk), observer(std::move(obs)) {
        processor.subscribe(observer);
    }

    void append_and_process(const char *data, std::size_t size) {
        for (std::size_t i = 0; i < size; ++i) {
            const char c = data[i];
            if (c == '\n') {
                if (!linebuf.empty() && linebuf.back() == '\r') {
                    linebuf.pop_back();
                }
                processor.processLine(linebuf);
                linebuf.clear();
            } else {
                linebuf.push_back(c);
            }
        }
    }

    void flush_pending_line() {
        if (!linebuf.empty()) {
            if (linebuf.back() == '\r') {
                linebuf.pop_back();
            }
            processor.processLine(linebuf);
            linebuf.clear();
        }
    }

    std::mutex mtx;
    CommandProcessor processor;
    std::string linebuf;
    std::shared_ptr<QueueingObserver> observer;
};

} // namespace

namespace async {

handle_t connect(std::size_t bulk) {
    AsyncRuntime::instance().observer();
    auto *ctx = new AsyncContext(bulk, AsyncRuntime::instance().observer());
    return ctx;
}

void receive(handle_t ctx, const char *data, std::size_t size) {
    if (ctx == nullptr || data == nullptr || size == 0) {
        return;
    }
    auto *c = static_cast<AsyncContext *>(ctx);
    std::lock_guard<std::mutex> lock(c->mtx);
    c->append_and_process(data, size);
}

void disconnect(handle_t ctx) {
    if (ctx == nullptr) {
        return;
    }
    auto *c = static_cast<AsyncContext *>(ctx);
    {
        std::lock_guard<std::mutex> lock(c->mtx);
        c->flush_pending_line();
        c->processor.finish();
    }
    delete c;
}

} // namespace async
