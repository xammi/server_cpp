#include "server.hpp"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace http {

using std::string;
using std::size_t;
using std::vector;

using boost::thread;

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

    signals_.async_wait(boost::bind(& Server::handle_stop, this));

    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::resolver::query query(address, port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    start_accept();
}

void Server::run() {
    vector<boost::shared_ptr<boost::thread> > threads;

    for (std::size_t i = 0; i < thread_pool_size_; ++i) {
        boost::shared_ptr<boost::thread> thread(new boost::thread(
            boost::bind(& boost::asio::io_service::run, & io_service_)));
        threads.push_back(thread);
    }

    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();
}

void Server::start_accept() {
    new_connection_.reset(new Connection(io_service_, request_handler_));
    acceptor_.async_accept(new_connection_->socket(),
        boost::bind(& Server::handle_accept, this,
            boost::asio::placeholders::error));
}

void Server::handle_accept(const boost::system::error_code & error) {
    if (! error) {
        new_connection_->start();
    }
    start_accept();
}

void Server::handle_stop() {
    io_service_.stop();
}

} // namespace http
