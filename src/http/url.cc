#include "url.h"
#include "util/expected.h"
#include "util/string.h"
#include <string>
#include <string_view>

namespace http {

auto URL::operator==(const URL& rhs) const noexcept -> bool {
    return this->scheme == rhs.scheme &&
           this->opaque == rhs.opaque &&
           this->userinfo == rhs.userinfo &&
           this->host == rhs.host &&
           this->path == rhs.path &&
           this->query == rhs.query &&
           this->fragment == rhs.fragment;
}

namespace {

constexpr inline
auto isAlphaSym(const char c) -> bool {
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z');
}

constexpr inline
auto isDigitSym(const char c) -> bool {
    return '0' <= c && c <= '9';
}

constexpr inline
auto isUnreservedSym(const char c) -> bool {
    return isAlphaSym(c) ||
           isDigitSym(c) ||
           c == '-' ||
           c == '.' ||
           c == '_' ||
           c == '~';
}

constexpr inline
auto isSubdelimSym(const char c) -> bool {
    return c == '!' ||
           c == '$' ||
           c == '&' ||
           c == '\'' ||
           c == '(' ||
           c == ')' ||
           c == '*' ||
           c == '+' ||
           c == ',' ||
           c == ';' ||
           c == '=';
}

constexpr inline
auto isPcharSym(const char c) -> bool {
    return isUnreservedSym(c) ||
           isSubdelimSym(c) ||
           c == '%' ||
           c == ':' ||
           c == '@';
}

/**
 * Try to parse scheme from raw url string.
 * Returns:
 *  .0 - string part with parsed scheme.
 *  .1 - remaining part of the url string.
 *  .2 - error C string, must be NULL if there are no error.
 */
auto getScheme(std::string_view raw_url) noexcept
    -> std::tuple<std::string_view, std::string_view, UrlError> {

    size_t i = 0;
    for (; i < raw_url.size(); ++i) {
        const char c = raw_url[i];
        if (!(isAlphaSym(c) || isDigitSym(c) || c == '+' || c == '-' || c == '.')) {
            break;
        }
    }

    if (i >= raw_url.size()) {
        return { "", raw_url, NO_ERROR };
    }

    if (raw_url[i] == ':') {
        if (i == 0)
            return { "", "", EMPTY_SCHEME_ERROR };
        if (!isAlphaSym(raw_url[0]))
            return { "", raw_url, NO_ERROR };
        return { raw_url.substr(0, i), raw_url.substr(i+1), NO_ERROR };
    }

    return { "", raw_url, NO_ERROR };
}

/**
 * Try to parse fragment part from raw url string.
 * Returns:
 *  .0 - string part with parsed fragment.
 *  .1 - remaining part of the url string.
 */
auto getFragment(std::string_view raw_url) noexcept
    -> std::tuple<std::string_view, std::string_view> {

    auto i = raw_url.rfind('#');
    if (i == std::string::npos) {
        return { "", raw_url };
    }

    return { raw_url.substr(i+1), raw_url.substr(0, i) };
}

/**
 * Try to parse query part from url string (must be without fragment part).
 * Returns:
 *  .0 - string part with parsed query.
 *  .1 - remaining part of the url string.
 */
auto getQuery(std::string_view raw_url) noexcept
    -> std::tuple<std::string_view, std::string_view> {

    auto i = raw_url.rfind('?');
    if (i == std::string::npos) {
        return { "", raw_url };
    }

    return { raw_url.substr(i+1), raw_url.substr(0, i) };
}

/**
 * Try to parse an authtority from a string.
 * Returns:
 *  .0 - userinfo part of the string
 *  .1 - host part of the string
 */
auto parseAuthority(std::string_view str) noexcept
    -> std::tuple<std::string_view, std::string_view> {
    auto i = str.find('@');

    if (i == std::string::npos) {
        return { "", str };
    }
    return { str.substr(0, i), str.substr(i+1) };
}

}  // namespace http::

/**
 *  Set a scheme and validate it
 */
auto URL::setScheme(std::string_view s) noexcept -> UrlError {
    if (s.empty()) {
        return EMPTY_SCHEME_ERROR;
    }

    for (auto c: s) {
        if (!(isAlphaSym(c) || isDigitSym(c) || c == '+' || c == '-' || c == '.'))
            return INVALID_SCHEME_ERROR;
    }

    if (!isAlphaSym(s[0])) {
        return INVALID_SCHEME_ERROR;
    }

    this->scheme = toLower(s);
    return NO_ERROR;
}

/**
 *  Set an opaque and validate it
 */
auto URL::setOpaque(std::string_view s) noexcept -> UrlError {
    if (s.empty())
        return EMPTY_OPAQUE_ERROR;

    for (auto c: s) {
        if (!isPcharSym(c))
            return INVALID_OPAQUE_ERROR;
    }
    this->opaque = s;
    return NO_ERROR;
}

/**
 *  Set an userinfo and validate it
 */
auto URL::setUserinfo(std::string_view u) noexcept -> UrlError {
    this->userinfo = u;
    return NO_ERROR;
}

/**
 *  Set a host and validate it
 */
auto URL::setHost(std::string_view h) noexcept -> UrlError {
    this->host = h;
    return NO_ERROR;
}

/**
 *  Set a path and validate it
 */
auto URL::setPath(std::string_view p) noexcept -> UrlError {
    this->path = p;
    return NO_ERROR;
}

/**
 *  Set a query and validate it
 */
auto URL::setQuery(std::string_view q) noexcept -> UrlError {
    this->query = q;
    return NO_ERROR;
}

/**
 *  Set a fragment and validate it
 */
auto URL::setFragment(std::string_view f) noexcept -> UrlError {
    this->fragment = f;
    return NO_ERROR;
}

/**
 * Parse URL from a string. Url could be absolute (starts with a protocol scheme) or
 * relative (starts with a path or an opaque).
 *
 * Returns:
 *  an URL structure or a description of the parsing error
 */
auto parseRequestURL(const std::string& raw_url) noexcept -> Expected<URL, UrlError> {
    URL url;

    if (raw_url.empty()) {
        return { UNEXPECTED, ROOTLESS_PATH_ERROR };
    } else if (raw_url == "*") {
        url.path = "*";
    } else {
        std::string_view to_parse = raw_url;
        // cut fragment
        {
            auto [fragment_str, remaining] = getFragment(to_parse);
            url.setFragment(fragment_str);
            to_parse = remaining;
        }
        // cut query
        {
            auto [query_str, remaining] = getQuery(to_parse);
            url.setQuery(query_str);
            to_parse = remaining;
        }
        // cut scheme
        {
            auto [scheme_str, remaining, error] = getScheme(to_parse);
            if (error != NO_ERROR) {
                return { UNEXPECTED, error };
            }
            url.scheme = toLower(scheme_str);
            to_parse = remaining;
        }

        // cut opaque
        if (!url.scheme.empty() && !to_parse.empty() && to_parse[0] != '/') {
            url.setOpaque(to_parse);
            return url;
        }

        // cut authority
        if (to_parse.substr(0, 2) == "//") {
            to_parse = to_parse.substr(2);

            UrlError error = NO_ERROR;
            size_t auth_end = std::min(to_parse.find('/'), to_parse.size());

            auto [user_str, host_str] = parseAuthority(to_parse.substr(0, auth_end));

            if ((error = url.setUserinfo(user_str)) != NO_ERROR) {
                return { UNEXPECTED, error };
            }
            if ((error = url.setHost(host_str)) != NO_ERROR) {
                return { UNEXPECTED, error };
            }

            to_parse = to_parse.substr(auth_end);
        }

        // path is the last one to parse
        if (url.scheme.empty() && (to_parse.empty() || to_parse[0] != '/')) {
            // rootless path for empty scheme is forbidden at request URL
            return { UNEXPECTED, ROOTLESS_PATH_ERROR };
        } else {
            auto error = url.setPath(to_parse);
            if (error != NO_ERROR) {
                return { UNEXPECTED, error };
            }
        }
    }
    return url;
}

}  // namepsace http
