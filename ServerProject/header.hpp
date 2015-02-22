#ifndef HEADER_HPP
#define HEADER_HPP

#include <string>

namespace http {

using std::string;

struct header {
    header(const string & _name, const string & _value)
        : name(_name), value(_value)
    {}

    string name;
    string value;
};

} // namespace http

#endif // HEADER_HPP
