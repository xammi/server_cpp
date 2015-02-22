#include "mime_types.hpp"

namespace http {
namespace mime_types {

string extension_to_type(const string & extension) {
    switch (extension) {
        case "json": return "application/json";
        case "js": return "application/javascript";
        case "pdf": return "application/pdf";

        case "mp3": return "audio/mp3";
        case "mp4": return "video/mp4";

        case "jpg": return "image/jpeg";
        case "png": return "image/png";
        case "gif": return "image/gif";

        case "htm": return "text/html";
        case "html": return "text/html";
        case "css": return "text/css";
        case "xml": return "text/xml";
    }
    return "text/plain";
}

} // namespace mime_types
} // namespace http
