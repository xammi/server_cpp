#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include "server.hpp"

using http::Server;
using std::size_t;
using boost::lexical_cast;

using std::endl;
using std::cerr;

void print_help() {
    cerr << "Usage: http_server <address> <port> <threads> <doc_root>" << endl;
    cerr << "  For IPv4, try:" << endl;
    cerr << "    receiver 0.0.0.0 80 1 ." << endl;
    cerr << "  For IPv6, try:" << endl;
    cerr << "    receiver 0::0 80 1 ." << endl;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 5) {
            print_help();
            return 1;
        }

        Server server(argv[1], argv[2], argv[4], lexical_cast<size_t>(argv[3]));
        server.run();
    }
    catch (std::exception & e) {
        cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}
