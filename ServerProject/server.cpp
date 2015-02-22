#include "server.hpp"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace http {

using boost::thread;
using boost::bind;
namespace placeholders = boost::asio::placeholders;

Server::Server(const string & address, const string & port, const string & doc_root, size_t thread_pool_size)
    : thread_pool_size_(thread_pool_size),
      signals_(io_service_),
      acceptor_(io_service_),
      new_connection_(),
      request_handler_(doc_root)
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
    vector<shared_ptr<thread> > threads;

    for (std::size_t i = 0; i < thread_pool_size_; ++i) {
        shared_ptr<thread> thread_ptr(new thread(bind(& io_service::run, & io_service_)));
        threads.push_back(thread_ptr);
    }

    for (size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();
}

void Server::start_accept() {
    new_connection_.reset(new Connection(io_service_, request_handler_));
    acceptor_.async_accept(new_connection_->get_socket(),
                           bind(& Server::handle_accept, this, placeholders::error));
}

void Server::handle_accept(const error_code & error) {
    if (! error) {
        new_connection_->start();
    }
    start_accept();
}

void Server::handle_stop() {
    io_service_.stop();
}

} // namespace http
