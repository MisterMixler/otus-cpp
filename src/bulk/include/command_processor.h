#pragma once

#include "observer.h"

#include <cstddef>
#include <memory>
#include <vector>

class CommandProcessor {
public:
    explicit CommandProcessor(std::size_t block_size);

    void processLine(const std::string& line);
    void finish();

    void subscribe(std::shared_ptr<IObserver> observer);

private:
    void flushBlock();

    std::size_t block_size_;
    Block current_block_;
    std::size_t brace_depth_{0};
    std::vector<std::weak_ptr<IObserver>> observers_;
};
