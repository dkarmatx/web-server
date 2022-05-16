#include "url.h"
#include "util/expected.h"
#include "util/string.h"
#include "util/charset.h"

#include <string>
#include <string_view>

namespace http {

const static auto alphaCset       = CharSet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
const static auto digitCset       = CharSet("0123456789");
const static auto subdelimsCset   = CharSet("!$&'()*+,;=");
const static auto unreservedCset  = CharSet("-._~") + alphaCset + digitCset;
const static auto phcarCset       = CharSet("%:@") + unreservedCset + subdelimsCset;

const static auto opaqueCset      = phcarCset + CharSet("/");
const static auto& pathCset       = opaqueCset;
const static auto schemeCset      = CharSet("+-.") + alphaCset + digitCset;
const static auto userinfoCset    = CharSet("%:") + unreservedCset + subdelimsCset;
const static auto queryCset       = CharSet("/?[]") + phcarCset;

const static auto regnameCset     = unreservedCset + subdelimsCset + CharSet("%");
const static auto& portCset       = digitCset;

/**
 *  Compares URL structures.
 */
auto URL::operator==(const URL& rhs) const noexcept -> bool {
    return this->scheme == rhs.scheme &&
           this->opaque == rhs.opaque &&
           this->userinfo == rhs.userinfo &&
           this->host == rhs.host &&
           this->path == rhs.path &&
           this->query == rhs.query &&
           this->fragment == rhs.fragment;
}

/**
 *  Set a scheme and validate it
 */
auto URL::setScheme(std::string_view s) noexcept -> UrlError {
    if (s.empty()) {
        return EMPTY_SCHEME_ERROR;
    }

    if (!alphaCset.has(s.front())) {
        return INVALID_SCHEME_ERROR;
    }

    if (!schemeCset.has(s)) {
        return INVALID_SCHEME_ERROR;
    }

    this->scheme = toLower(s);
    return NO_ERROR;
}

/**
 *  Set an opaque and validate it.
 *  Function doesn't validate url-pct-encoding.
 */
auto URL::setOpaque(std::string_view s) noexcept -> UrlError {
    if (!opaqueCset.has(s))
        return INVALID_OPAQUE_ERROR;

    this->opaque = s;
    return NO_ERROR;
}

/**
 *  Set an userinfo and validate it.
 *  Function doesn't validate url-pct-encoding.
 */
auto URL::setUserinfo(std::string_view u) noexcept -> UrlError {
    if (!userinfoCset.has(u))
        return INVALID_USERINFO_ERROR;

    this->userinfo = u;
    return NO_ERROR;
}

/**
 *  Set a host and validate it
 */
auto URL::setHost(std::string_view h) noexcept -> UrlError {
    // return INVALID_HOSTNAME_ERROR;
    // return EMPTY_PORT_ERROR;
    // return INVALID_PORT_ERROR;

    // make host validation

    this->host = h;
    return NO_ERROR;
}

/**
 *  Set a path and validate it. If path is empty, then
 *  this object's path is set to "/". Rootless path isn't allowed.
 *  Function doesn't validate url-pct-encoding.
 */
auto URL::setPath(std::string_view p) noexcept -> UrlError {
    if (p.empty())
        return ROOTLESS_PATH_ERROR;

    if (p.front() != '/')
        return ROOTLESS_PATH_ERROR;
    
    for (auto c: p) {
        if (!pathCset.has(c)) {
            return INVALID_PATH_ERROR;
        }
    }

    this->path = p;
    return NO_ERROR;
}

/**
 *  Set a query and validate it.
 *  Function doesn't validate url-pct-encoding.
 */
auto URL::setQuery(std::string_view q) noexcept -> UrlError {
    for (auto c: q) {
        if (!queryCset.has(c)) {
            return INVALID_QUERY_ERROR;
        }
    }
    this->query = q;
    return NO_ERROR;
}

/**
 * Parse URL from a string. Url could be absolute (starts with a protocol scheme) or
 * relative (starts with a path or an opaque).
 *
 * Returns:
 *  an URL structure or a description of the parsing error
 */
auto parseRequestURL(const std::string&) noexcept -> Expected<URL, UrlError> {
    return URL{};
}

}  // namepsace http
