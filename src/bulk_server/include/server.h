#pragma once

#include "output.h"
#include "shared_processor.h"

#include <cstddef>
#include <memory>

#include <boost/asio.hpp>

class Server {
public:
    Server(boost::asio::io_context &io, unsigned short port, std::size_t bulk_size);

private:
    void do_accept();

    boost::asio::ip::tcp::acceptor acceptor_;
    std::shared_ptr<OutputObserver> observer_;
    SharedProcessor shared_;
};
