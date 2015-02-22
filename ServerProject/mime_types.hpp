#ifndef MIME_TYPES_HPP
#define MIME_TYPES_HPP

#include <string>

namespace http {
namespace mime_types {

using std::string;

string extension_to_type(const string & extension);

} // namespace mime_types
} // namespace http

#endif // MIME_TYPES_HPP
