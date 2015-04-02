#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server.hpp"
#include <boost/program_options.hpp>

using http::Server;
using std::string;
using std::cout;
using std::endl;

namespace po = boost::program_options;

void description(po::options_description & meta) {
    meta.add_options()
            ("help,h", "produce help message")
            ("address,a",
             po::value<string>()->required(),
             "Binded server address"
            )
            ("port,p",
             po::value<string>()->default_value("5005"),
             "Listened server port"
            )
            ("doc-root,r",
             po::value<string>()->default_value("."),
             "Document root"
            );
}

int main(int argc, char* argv[]) {
    try {
        po::options_description meta;
        description(meta);
        po::variables_map vars;

        po::store(po::parse_command_line(argc, argv, meta), vars);
        if (vars.count("help")) {
            cout << meta << endl;
            return 0;
        }
        po::notify(vars);

        const string & address = vars["address"].as<string>();
        const string & port = vars["port"].as<string>();
        const string & doc_root = vars["doc-root"].as<string>();

        Server server(address, port, doc_root);
        cout << "Server started at " << address << ":" << port << endl;
        server.run();
    }
    catch (std::exception & e) {
        std::cerr << "exception: " << e.what() << endl;
    }

    return 0;
}
