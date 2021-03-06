#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "connection.hpp"
#include "request_handler.hpp"

namespace http {

using boost::asio::signal_set;
using boost::thread;

typedef shared_ptr<thread> ThreadPtr;

class Server : private boost::noncopyable
{

public:
    Server(const string & address, const string & port, const string & doc_root);
    ~Server() {}

    void run();

private:
    void start_accept();
    void handle_accept(const error_code &);
    void handle_stop();

private:
    io_service io_service_;
    signal_set signals_;
    tcp::acceptor acceptor_;

    ConnectionPtr new_connection;
    RequestHandler request_handler;
};

} // namespace http

#endif // SERVER_HPP
