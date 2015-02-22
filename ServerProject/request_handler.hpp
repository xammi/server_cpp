#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <string>
#include <boost/noncopyable.hpp>

namespace http {

using std::string;

struct Response;
struct Request;

class RequestHandler : private boost::noncopyable
{
public:
    explicit RequestHandler(const string & doc_root);

    void handle_request(const Request & req, Response & rep);

private:
    static bool url_decode(const string & in, string & out);

    string doc_root_;
};

} // namespace http

#endif // REQUEST_HANDLER_HPP
