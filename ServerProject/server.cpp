#include "server.hpp"
#include <boost/bind.hpp>
#include <vector>

namespace http {

using boost::bind;
namespace placeholders = boost::asio::placeholders;

Server::Server(const string & address, const string & port, const string & doc_root, int thread_pool_size_)
    : thread_pool_size(thread_pool_size_),
      io_service_(),
      signals_(io_service_),
      acceptor_(io_service_),
      new_connection(),
      request_handler(doc_root)
{
    signals_.add(SIGINT);
    signals_.add(SIGTERM);

#if defined(SIGQUIT)
    signals_.add(SIGQUIT);
#endif

    signals_.async_wait(bind(& Server::handle_stop, this));

    tcp::resolver resolver(io_service_);
    tcp::resolver::query query(address, port);
    tcp::endpoint endpoint = *resolver.resolve(query);

    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    start_accept();
}

void Server::run() {
    for (int i = 0; i < thread_pool_size; ++i) {
        ThreadPtr thread_ptr(new thread(bind(& io_service::run, & io_service_)));
        pool.push_back(thread_ptr);
    }
}

void Server::join() {
    for (ThreadPtr & thread_ptr : pool)
        thread_ptr->join();
}

void Server::start_accept() {
    new_connection.reset(new Connection(io_service_, request_handler));
    acceptor_.async_accept(new_connection->get_socket(),
                           bind(& Server::handle_accept, this, placeholders::error));
}

void Server::handle_accept(const error_code & error) {
    if (! error) {
        new_connection->start();
    }
    start_accept();
}

void Server::handle_stop() {
    io_service_.stop();
}

} // namespace http
