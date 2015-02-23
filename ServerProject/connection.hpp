#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "response.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include "request_parser.hpp"

namespace http {

using boost::system::error_code;
using boost::asio::ip::tcp;
using boost::asio::io_service;
using boost::shared_ptr;

typedef shared_ptr<Connection> ConnectionPtr;

class Connection
        : public boost::enable_shared_from_this<Connection>,
          private boost::noncopyable
{
public:
    Connection(io_service & io_service_, RequestHandler & handler);

    tcp::socket & get_socket();

    void start();

private:

    void handle_read(const error_code &, size_t bytes_transferred);
    void handle_write(const error_code &);

    io_service::strand strand_;
    tcp::socket socket_;
    boost::array<char, 8192> buffer_;

    RequestHandler & request_handler;
    Request request;
    RequestParser request_parser;
    Response response;
};

} // namespace http

#endif // CONNECTION_HPP
