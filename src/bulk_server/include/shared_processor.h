#pragma once

#include "command_processor.h"
#include "observer.h"

#include <memory>
#include <string>

// Shared processor for static (non-braced) commands from all connections.
// All access is serialised by the single io_context thread.
class SharedProcessor {
public:
    SharedProcessor(std::size_t bulk_size, std::shared_ptr<IObserver> obs)
        : processor_(bulk_size), observer_(std::move(obs)) {
        processor_.subscribe(observer_);
    }

    void processLine(const std::string &line) { processor_.processLine(line); }
    void finish() { processor_.finish(); }

private:
    CommandProcessor processor_;
    std::shared_ptr<IObserver> observer_;
};
