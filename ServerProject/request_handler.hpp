#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <string>
#include <boost/noncopyable.hpp>

namespace http {

using std::string;
using std::size_t;

struct Response;
struct Request;

class RequestHandler : private boost::noncopyable
{
public:
    explicit RequestHandler(const string &);

    void handle_request(const Request & req, Response & rep);

private:
    static bool url_decode(const string & in, string & out);

private:
    string doc_root;
};

} // namespace http

#endif // REQUEST_HANDLER_HPP
