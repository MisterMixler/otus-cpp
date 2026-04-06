#include "file_logger.h"

#include <fstream>

void FileLogger::onBlock(const Block& block) {
    auto filename = "bulk" + std::to_string(block.timestamp) + ".log";
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
