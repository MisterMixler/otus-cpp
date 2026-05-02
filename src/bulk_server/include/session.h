#pragma once

#include "observer.h"
#include "shared_processor.h"

#include <array>
#include <memory>
#include <string>

#include <boost/asio.hpp>

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::ip::tcp::socket socket,
            SharedProcessor &shared,
            std::shared_ptr<IObserver> obs);

    void start();

private:
    void do_read();
    void process_data(const char *data, std::size_t size);
    void process_line(const std::string &line);
    void flush_dynamic_block();
    void on_disconnect();

    boost::asio::ip::tcp::socket socket_;
    SharedProcessor &shared_;
    std::shared_ptr<IObserver> observer_;

    std::array<char, 4096> buf_{};
    std::string linebuf_;
    std::size_t brace_depth_{0};
    Block dynamic_block_;
};
