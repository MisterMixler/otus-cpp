#include "command_processor.h"
#include "console_logger.h"
#include "file_logger.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: bulk <block_size>" << std::endl;
        return 1;
    }

    auto block_size = std::stoul(argv[1]);

    CommandProcessor processor(block_size);

    auto console = std::make_shared<ConsoleLogger>();
    auto file_log = std::make_shared<FileLogger>();

    processor.subscribe(console);
    processor.subscribe(file_log);

    std::string line;
    while (std::getline(std::cin, line))
        processor.processLine(line);

    processor.finish();

    return 0;
}
