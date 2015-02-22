#ifndef REPLY_HPP
#define REPLY_HPP

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include "header.hpp"

namespace http {

using std::string;
using std::vector;
using boost::asio::const_buffer;

struct Response {

    enum status_type {
        ok = 200,
        created = 201,
        accepted = 202,
        no_content = 204,
        multiple_choices = 300,
        moved_permanently = 301,
        moved_temporarily = 302,
        not_modified = 304,
        bad_request = 400,
        unauthorized = 401,
        forbidden = 403,
        not_found = 404,
        internal_server_error = 500,
        not_implemented = 501,
        bad_gateway = 502,
        service_unavailable = 503
    } status;

    vector<Header> headers;
    string content;
    vector<const_buffer> to_buffers();

    static string get_http_date_now();
    static Response ok_reply(const string & extension);
    static Response stock_reply(status_type status);
};

} // namespace http

#endif // REPLY_HPP