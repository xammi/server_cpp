#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

namespace http {

using boost::tuples::tuple;
using boost::logic::tribool;
using boost::make_tuple;
using boost::logic::indeterminate;

struct Request;

class RequestParser
{
public:
    RequestParser();

    void reset();

    template <typename InputIterator>
    tuple<tribool, InputIterator> parse(Request & req, InputIterator begin, InputIterator end) {

        while (begin != end) {
            tribool result = consume(req, *begin++);
            if (result || !result)
                return make_tuple(result, begin);
        }
        tribool result = indeterminate;
        return make_tuple(result, begin);
    }

private:
    tribool consume(Request & req, char input);

    static bool is_char(int c);
    static bool is_ctl(int c);
    static bool is_tspecial(int c);
    static bool is_digit(int c);

    enum State {
        method_start,
        method,
        uri,
        http_version_h,
        http_version_t_1,
        http_version_t_2,
        http_version_p,
        http_version_slash,
        http_version_major_start,
        http_version_major,
        http_version_minor_start,
        http_version_minor,
        expecting_newline_1,
        header_line_start,
        header_lws,
        header_name,
        space_before_header_value,
        header_value,
        expecting_newline_2,
        expecting_newline_3
    } state;
};

} // namespace http

#endif // REQUEST_PARSER_HPP
