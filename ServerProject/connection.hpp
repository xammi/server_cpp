#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "reply.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include "request_parser.hpp"

namespace http {

class Connection
        : public boost::enable_shared_from_this<Connection>,
          private boost::noncopyable
{
public:
    explicit Connection(boost::asio::io_service & io_service, RequestHandler & handler);

    boost::asio::ip::tcp::socket & socket();

    void start();

private:

    void handle_read(const boost::system::error_code &, std::size_t bytes_transferred);
    void handle_write(const boost::system::error_code &);

    boost::asio::io_service::strand strand_;
    boost::asio::ip::tcp::socket socket_;
    boost::array<char, 8192> buffer_;

    RequestHandler & request_handler;
    Request request;
    RequestParser request_parser;
    Reply reply;
};

typedef boost::shared_ptr<Connection> ConnectionPtr;

} // namespace http

#endif // CONNECTION_HPP
