#include "mime_types.hpp"
#include <map>

namespace http {
namespace mime_types {


std::map<string, string> mappings = {

    {"html", "text/html"},
    {"js", "application/javascript"},
    {"css", "text/css"},

    {"htm", "text/html"},
    {"xml", "text/xml"},

    {"jpg", "image/jpeg"},
    {"jpeg", "image/jpeg"},
    {"png", "image/png"},
    {"gif", "image/gif"},

    {"json", "application/json"},
    {"swf", "application/x-shockwave-flash"},
    {"pdf", "application/pdf"},

    {"mp3", "audio/mpeg3"},
};

string extension_to_type(const string & extension) {
    auto it = mappings.find(extension);
    if (it != mappings.end())
        return it->second;
    return "text/plain";
}

} // namespace mime_types
} // namespace http
