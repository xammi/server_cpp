#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "connection.hpp"
#include "request_handler.hpp"

namespace http {

using std::string;
using std::size_t;

class Server : private boost::noncopyable
{
public:
    explicit Server(const string & address, const string & port, const string & doc_root, size_t thread_pool_size);
    ~Server() {}

    void run();

private:
    void start_accept();
    void handle_accept(const boost::system::error_code &);
    void handle_stop();

private:
    size_t thread_pool_size_;

    boost::asio::io_service io_service_;
    boost::asio::signal_set signals_;
    boost::asio::ip::tcp::acceptor acceptor_;

    ConnectionPtr new_connection_;
    RequestHandler request_handler_;
};

} // namespace http

#endif // SERVER_HPP
