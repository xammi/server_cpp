#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>

#include "mime_types.hpp"
#include "response.hpp"
#include "request.hpp"
#include <boost/algorithm/string.hpp>

namespace http {

using boost::iequals;

RequestHandler::RequestHandler(const string & doc_root_)
  : doc_root(doc_root_)
{}

void RequestHandler::handle_request(const Request & req, Response & res) {
    if (!iequals(req.method, "GET") && !iequals(req.method, "HEAD")) {
        res = Response::stock_reply(Response::not_allowed);
        return;
    }

    string request_path;
    if (!url_decode(req.uri, request_path)) {
        res = Response::stock_reply(Response::bad_request);
        return;
    }

    if (request_path.empty() || request_path[0] != '/' || request_path.find("..") != string::npos) {
        res = Response::stock_reply(Response::bad_request);
        return;
    }

    bool req_index = false;
    if (request_path[request_path.size() - 1] == '/') {
        request_path += "index.html";
        req_index = true;
    }

    size_t last_slash_pos = request_path.find_last_of("/");
    size_t last_dot_pos = request_path.find_last_of(".");

    string extension;
    if (last_dot_pos != string::npos && last_dot_pos > last_slash_pos) {
        extension = request_path.substr(last_dot_pos + 1);
    }

    string full_path = doc_root + request_path;
    // std::cout << req.method << " " << full_path << std::endl;

    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
    if (!is) {
        if (req_index)
            res = Response::stock_reply(Response::forbidden);
        else
            res = Response::stock_reply(Response::not_found);
        return;
    }

    res = Response::ok_reply(extension);
    if (iequals(req.method, "HEAD")) {
        is.seekg(0, std::ios_base::end);
        res.setContentLength(is.tellg());
    }
    else if (iequals(req.method, "GET")) {
        char buf[124];
        while (is.read(buf, sizeof(buf)).gcount() > 0)
            res.content.append(buf, is.gcount());
        res.updateContentLength();
    }
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
                else
                    return false;
            }
            else
                return false;
        }
        else if (in[i] == '+')
            out += ' ';
        else if (in[i] == '?')
            return true; // parse GET parameters
        else
            out += in[i];
    }
    return true;
}

} // namespace http
