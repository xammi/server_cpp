#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>

namespace http {

using boost::bind;
using boost::asio::async_write;

namespace placeholders = boost::asio::placeholders;

Connection::Connection(io_service & io_service_, RequestHandler & handler)
    : strand_(io_service_), socket_(io_service_), request_handler(handler)
{}

tcp::socket & Connection::get_socket() {
    return socket_;
}

void Connection::start() {
    socket_.async_read_some(buffer(buffer_),
        strand_.wrap(
            bind(
                & Connection::handle_read,
                shared_from_this(),
                placeholders::error,
                placeholders::bytes_transferred
            )));
}

void Connection::handle_read(const error_code & e, size_t bytes_transferred) {
    if (!e) {
        tribool result;
        boost::tie(result, boost::tuples::ignore) = request_parser.parse(request, buffer_.data(), buffer_.data() + bytes_transferred);

        if (result) {
            request_handler.handle_request(request, response);
            async_write(socket_, response.to_buffers(),
                strand_.wrap(
                    bind(
                        & Connection::handle_write,
                        shared_from_this(),
                        placeholders::error
                    )));
        }
        else if (!result) {
            response = Response::stock_reply(Response::bad_request);
            async_write(socket_, response.to_buffers(),
                strand_.wrap(
                    bind(
                        & Connection::handle_write,
                        shared_from_this(),
                        placeholders::error
                    )));
        }
        else {
            socket_.async_read_some(buffer(buffer_),
                strand_.wrap(
                    bind(
                        & Connection::handle_read,
                        shared_from_this(),
                        placeholders::error,
                        placeholders::bytes_transferred
                    )));
        }
    }
}

void Connection::handle_write(const error_code & e) {
    if (!e) {
        error_code ignored_ec;
        socket_.shutdown(tcp::socket::shutdown_both, ignored_ec);
    }
}

} // namespace http
