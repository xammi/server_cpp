#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <string>
#include <boost/noncopyable.hpp>

namespace http {

struct Reply;
struct Request;

// The common handler for all incoming requests.
class RequestHandler
        : private boost::noncopyable
{
public:
    // Construct with a directory containing files to be served.
    explicit RequestHandler(const std::string & doc_root);

    // Handle a request and produce a reply.
    void handle_request(const Request & req, Reply & rep);

private:
    // The directory containing the files to be served.
    std::string doc_root_;

    // Perform URL-decoding on a string. Returns false if the encoding was
    // invalid.
    static bool url_decode(const std::string & in, std::string & out);
};

} // namespace http

#endif // REQUEST_HANDLER_HPP
