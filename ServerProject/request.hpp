#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector>
#include "header.hpp"

namespace http {

using std::string;
using std::vector;

struct Request {
    string method;
    string uri;

    int http_version_major;
    int http_version_minor;

    vector<Header> headers;
};

} // namespace http


#endif // REQUEST_H
