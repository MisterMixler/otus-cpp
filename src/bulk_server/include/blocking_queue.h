#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>

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
