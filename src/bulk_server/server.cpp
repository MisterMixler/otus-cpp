#include "server.h"
#include "session.h"

namespace net = boost::asio;
using tcp = net::ip::tcp;

Server::Server(net::io_context &io, unsigned short port, std::size_t bulk_size)
    : acceptor_(io, tcp::endpoint(tcp::v4(), port)),
      observer_(std::make_shared<OutputObserver>()),
      shared_(bulk_size, observer_) {
    do_accept();
}

void Server::do_accept() {
    acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec)
            std::make_shared<Session>(std::move(socket), shared_, observer_)->start();
        do_accept();
    });
}
