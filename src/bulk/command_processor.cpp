#include "command_processor.h"

CommandProcessor::CommandProcessor(std::size_t block_size)
    : block_size_(block_size) {}

void CommandProcessor::subscribe(std::shared_ptr<IObserver> observer) {
    observers_.push_back(observer);
}

void CommandProcessor::flushBlock() {
    if (current_block_.commands.empty())
        return;

    for (auto it = observers_.begin(); it != observers_.end();) {
        if (auto obs = it->lock()) {
            obs->onBlock(current_block_);
            ++it;
        } else {
            it = observers_.erase(it);
        }
    }
    current_block_ = Block{};
}

void CommandProcessor::processLine(const std::string& line) {
    if (line == "{") {
        if (brace_depth_ == 0)
            flushBlock();
        ++brace_depth_;
        return;
    }

    if (line == "}") {
        if (brace_depth_ > 0) {
            --brace_depth_;
            if (brace_depth_ == 0)
                flushBlock();
        }
        return;
    }

    if (current_block_.commands.empty())
        current_block_.timestamp = std::time(nullptr);

    current_block_.commands.push_back(line);

    if (brace_depth_ == 0 && current_block_.commands.size() == block_size_)
        flushBlock();
}

void CommandProcessor::finish() {
    if (brace_depth_ == 0)
        flushBlock();
}
