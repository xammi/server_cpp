#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "mime_types.hpp"
#include "response.hpp"
#include "request.hpp"

namespace http {

using std::string;
using std::size_t;

RequestHandler::RequestHandler(const string & doc_root)
  : doc_root_(doc_root)
{}

void RequestHandler::handle_request(const Request & req, Response & res) {
    string request_path;
    if (!url_decode(req.uri, request_path)) {
        res = Response::stock_reply(Response::bad_request);
        return;
    }

    if (request_path.empty() || request_path[0] != '/' || request_path.find("..") != string::npos) {
        res = Response::stock_reply(Response::bad_request);
        return;
    }

    if (request_path[request_path.size() - 1] == '/') {
        request_path += "index.html";
    }

    size_t last_slash_pos = request_path.find_last_of("/");
    size_t last_dot_pos = request_path.find_last_of(".");

    string extension;
    if (last_dot_pos != string::npos && last_dot_pos > last_slash_pos) {
        extension = request_path.substr(last_dot_pos + 1);
    }

    string full_path = doc_root_ + request_path;
    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
    if (!is) {
        res = Response::stock_reply(Response::not_found);
        return;
    }

    res.status = Response::ok;
    char buf[512];
    while (is.read(buf, sizeof(buf)).gcount() > 0)
        res.content.append(buf, is.gcount());

    res.headers = {
        header("Content-Length", boost::lexical_cast<std::string>(res.content.size())),
        header("Content-Type", mime_types::extension_to_type(extension)),
    };
}

bool RequestHandler::url_decode(const string & in, string & out) {
    out.clear();
    out.reserve(in.size());

    for (size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '%') {
            if (i + 3 <= in.size()) {
                int value = 0;
                std::istringstream is(in.substr(i + 1, 2));
                if (is >> std::hex >> value) {
                    out += static_cast<char>(value);
                    i += 2;
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }
        else if (in[i] == '+') {
            out += ' ';
        }
        else {
            out += in[i];
        }
    }

    return true;
}

} // namespace http
