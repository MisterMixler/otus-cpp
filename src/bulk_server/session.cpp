#include "session.h"

namespace net = boost::asio;

Session::Session(net::ip::tcp::socket socket,
                 SharedProcessor &shared,
                 std::shared_ptr<IObserver> obs)
    : socket_(std::move(socket)), shared_(shared), observer_(std::move(obs)) {}

void Session::start() { do_read(); }

void Session::do_read() {
    auto self = shared_from_this();
    socket_.async_read_some(
        net::buffer(buf_),
        [this, self](boost::system::error_code ec, std::size_t bytes) {
            if (!ec) {
                process_data(buf_.data(), bytes);
                do_read();
            } else {
                on_disconnect();
            }
        });
}

void Session::process_data(const char *data, std::size_t size) {
    for (std::size_t i = 0; i < size; ++i) {
        const char c = data[i];
        if (c == '\n') {
            if (!linebuf_.empty() && linebuf_.back() == '\r')
                linebuf_.pop_back();
            process_line(linebuf_);
            linebuf_.clear();
        } else {
            linebuf_.push_back(c);
        }
    }
}

void Session::process_line(const std::string &line) {
    if (line == "{") {
        ++brace_depth_;
        return;
    }

    if (line == "}") {
        if (brace_depth_ > 0) {
            --brace_depth_;
            if (brace_depth_ == 0)
                flush_dynamic_block();
        }
        return;
    }

    if (brace_depth_ == 0) {
        shared_.processLine(line);
    } else {
        if (dynamic_block_.commands.empty())
            dynamic_block_.timestamp = std::time(nullptr);
        dynamic_block_.commands.push_back(line);
    }
}

void Session::flush_dynamic_block() {
    if (dynamic_block_.commands.empty())
        return;
    observer_->onBlock(dynamic_block_);
    dynamic_block_ = Block{};
}

void Session::on_disconnect() {
    if (!linebuf_.empty()) {
        if (linebuf_.back() == '\r')
            linebuf_.pop_back();
        if (!linebuf_.empty())
            process_line(linebuf_);
        linebuf_.clear();
    }
    if (brace_depth_ == 0) {
        shared_.finish();
    }
}
