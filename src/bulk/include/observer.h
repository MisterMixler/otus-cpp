#pragma once

#include <ctime>
#include <string>
#include <vector>

struct Block {
    std::vector<std::string> commands;
    std::time_t timestamp{0};
};

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onBlock(const Block& block) = 0;
};
