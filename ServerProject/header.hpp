#ifndef HEADER_HPP
#define HEADER_HPP

#include <string>

namespace http {

struct header {
    header(const std::string & _name, const std::string & _value)
        : name(_name), value(_value)
    {}

    std::string name;
    std::string value;
};

} // namespace http

#endif // HEADER_HPP
