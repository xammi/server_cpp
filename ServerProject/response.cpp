#include "response.hpp"
#include <string>
#include <time.h>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "mime_types.hpp"

namespace http {

using std::string;
using std::vector;
using std::size_t;

using boost::asio::buffer;
using boost::asio::const_buffer;

namespace pt = boost::posix_time;

namespace status_strings {

const_buffer to_buffer(Response::status_type status) {
    switch (status)
    {
    case Response::ok:
        return buffer("HTTP/1.0 200 OK\r\n");
    case Response::created:
        return buffer("HTTP/1.0 201 Created\r\n");
    case Response::accepted:
        return buffer("HTTP/1.0 202 Accepted\r\n");
    case Response::no_content:
        return buffer("HTTP/1.0 204 No Content\r\n");
    case Response::multiple_choices:
        return buffer("HTTP/1.0 300 Multiple Choices\r\n");
    case Response::moved_permanently:
        return buffer("HTTP/1.0 301 Moved Permanently\r\n");
    case Response::moved_temporarily:
        return buffer("HTTP/1.0 302 Moved Temporarily\r\n");
    case Response::not_modified:
        return buffer("HTTP/1.0 304 Not Modified\r\n");
    case Response::bad_request:
        return buffer("HTTP/1.0 400 Bad Request\r\n");
    case Response::unauthorized:
        return buffer("HTTP/1.0 401 Unauthorized\r\n");
    case Response::forbidden:
        return buffer("HTTP/1.0 403 Forbidden\r\n");
    case Response::not_found:
        return buffer("HTTP/1.0 404 Not Found\r\n");
    case Response::internal_server_error:
        return buffer("HTTP/1.0 500 Internal Server Error\r\n");
    case Response::not_implemented:
        return buffer("HTTP/1.0 501 Not Implemented\r\n");
    case Response::bad_gateway:
        return buffer("HTTP/1.0 502 Bad Gateway\r\n");
    case Response::service_unavailable:
        return buffer("HTTP/1.0 503 Service Unavailable\r\n");
    default:
        return buffer("HTTP/1.0 500 Internal Server Error\r\n");
    }
}

} // namespace status_strings

namespace misc_strings {

const char name_value_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };

} // namespace misc_strings

vector<const_buffer> Response::to_buffers() {

    vector<const_buffer> buffers;
    buffers.push_back(status_strings::to_buffer(status));

    for (size_t i = 0; i < headers.size(); ++i) {
        Header & h = headers[i];
        buffers.push_back(buffer(h.name));
        buffers.push_back(buffer(misc_strings::name_value_separator));
        buffers.push_back(buffer(h.value));
        buffers.push_back(buffer(misc_strings::crlf));
    }

    buffers.push_back(buffer(misc_strings::crlf));
    buffers.push_back(buffer(content));
    return buffers;
}

namespace stock_replies {

#define wrapper(title, text) \
    "<html>" \
    "<head><title>" #title "</title></head>" \
    "<body><h1>" #text "</h1></body>" \
    "</html>"

string to_string(Response::status_type status) {
    switch (status) {
    case Response::ok:
        return "";
    case Response::created:
        return wrapper("Created", "201 Created");
    case Response::accepted:
        return wrapper("Accepted", "202 Accepted");
    case Response::no_content:
        return wrapper("No Content", "204 No Content");
    case Response::multiple_choices:
        return wrapper("Multiple Choices", "300 Multiple Choices");
    case Response::moved_permanently:
        return wrapper("Moved Permanently", "301 Moved Permanently");
    case Response::moved_temporarily:
        return wrapper("Moved Temporarily", "302 Moved Temporarily");
    case Response::not_modified:
        return wrapper("Not Modified", "304 Not Modified");
    case Response::bad_request:
        return wrapper("Bad Request", "400 Bad Request");
    case Response::unauthorized:
        return wrapper("Unauthorized", "401 Unauthorized");
    case Response::forbidden:
        return wrapper("Forbidden", "403 Forbidden");
    case Response::not_found:
        return wrapper("Not Found", "404 Not Found");
    case Response::internal_server_error:
        return wrapper("Internal Server Error", "500 Internal Server Error");
    case Response::not_implemented:
        return wrapper("Not Implemented", "501 Not Implemented");
    case Response::bad_gateway:
        return wrapper("Bad Gateway", "502 Bad Gateway");
    case Response::service_unavailable:
        return wrapper("Service Unavailable", "503 Service Unavailable");
    default:
        return wrapper("Internal Server Error", "500 Internal Server Error");;
    }
}

} // namespace stock_replies


string Response::get_http_date_now() {
    char buf[30];
    time_t now = time(0);
    struct tm tm = *gmtime(& now);

    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z\0", &tm);
    return string(buf);
}

Response Response::ok_reply(const string & extension) {
    Response res;
    res.status = Response::ok;
    res.content = "";

    res.headers = {
        {"Date", Response::get_http_date_now() },
        {"Server", "KMix boost 1.0"},
        {"Content-Length", boost::lexical_cast<std::string>(res.content.size())},
        {"Content-Type", mime_types::extension_to_type(extension)},
        {"Connection", "close"},
    };
    return res;
}

Response Response::stock_reply(Response::status_type status) {
    Response res;
    res.status = status;
    res.content = stock_replies::to_string(status);

    res.headers = {
        {"Date", Response::get_http_date_now() },
        {"Server", "KMix boost 1.0"},
        {"Content-Length", boost::lexical_cast<std::string>(res.content.size())},
        {"Content-Type", "text/html"},
        {"Connection", "close"},
    };
    return res;
}

} // namespace http
