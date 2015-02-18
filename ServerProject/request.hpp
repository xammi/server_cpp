#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector>
#include "header.hpp"

namespace http {

// A request received from a client.
struct Request {
    std::string method;
    std::string uri;

    int http_version_major;
    int http_version_minor;

    std::vector<header> headers;
};

} // namespace http


#endif // REQUEST_H
