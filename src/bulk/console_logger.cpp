#include "console_logger.h"

#include <iostream>

void ConsoleLogger::onBlock(const Block& block) {
    std::cout << "bulk: ";
    for (std::size_t i = 0; i < block.commands.size(); ++i) {
        if (i > 0)
            std::cout << ", ";
        std::cout << block.commands[i];
    }
    std::cout << std::endl;
}
