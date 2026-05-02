#include "output.h"
#include "server.h"

#include <cstdlib>
#include <iostream>
#include <thread>

#include <boost/asio.hpp>

namespace net = boost::asio;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: bulk_server <port> <bulk_size>" << std::endl;
        return 1;
    }

    const auto port = static_cast<unsigned short>(std::atoi(argv[1]));
    const auto bulk_size = static_cast<std::size_t>(std::atoi(argv[2]));

    std::thread log_thread(log_worker);
    std::thread file_thread_1(file_worker);
    std::thread file_thread_2(file_worker);

    try {
        net::io_context io;

        net::signal_set signals(io, SIGINT, SIGTERM);
        signals.async_wait([&io](boost::system::error_code, int) { io.stop(); });

        Server srv(io, port, bulk_size);
        io.run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    g_log_queue.shutdown();
    g_file_queue.shutdown();
    log_thread.join();
    file_thread_1.join();
    file_thread_2.join();

    return 0;
}
