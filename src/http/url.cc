#include "url.h"
#include "util/expected.h"
#include "util/string.h"
#include "util/charset.h"

#include <string>
#include <string_view>

namespace http {

static const auto alphaCset       = CharSet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
static const auto digitCset       = CharSet("0123456789");
static const auto subdelimsCset   = CharSet("!$&'()*+,;=");
static const auto unreservedCset  = CharSet("-._~") + alphaCset + digitCset;
static const auto phcarCset       = CharSet("%:@") + unreservedCset + subdelimsCset;
static const auto pathCset        = CharSet("/") + phcarCset;
static const auto schemeCset      = CharSet("+-.") + alphaCset + digitCset;
static const auto userinfoCset    = CharSet("%:") + unreservedCset + subdelimsCset;
static const auto queryCset       = CharSet("/?[]{}") + phcarCset;
static const auto regnameCset     = CharSet("%") + unreservedCset + subdelimsCset;

static const auto& opaqueCset     = pathCset;


auto URL::operator==(const URL& rhs) const -> bool {
    return this->scheme == rhs.scheme &&
           this->opaque == rhs.opaque &&
           this->userinfo == rhs.userinfo &&
           this->host == rhs.host &&
           this->path == rhs.path &&
           this->query == rhs.query &&
           this->fragment == rhs.fragment;
}

auto URL::setScheme(std::string_view s) -> UrlError {
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

auto URL::setOpaque(std::string_view s) -> UrlError {
    if (!opaqueCset.has(s))
        return INVALID_OPAQUE_ERROR;

    this->opaque = s;
    return NO_ERROR;
}

auto URL::setUserinfo(std::string_view u) -> UrlError {
    if (!userinfoCset.has(u))
        return INVALID_USERINFO_ERROR;

    this->userinfo = u;
    return NO_ERROR;
}

auto URL::setHost(std::string_view h) -> UrlError {
    // return INVALID_HOSTNAME_ERROR;
    // return EMPTY_PORT_ERROR;
    // return INVALID_PORT_ERROR;

    // make host validation

    this->host = h;
    return NO_ERROR;
}

auto URL::setPath(std::string_view p) -> UrlError {
    if (p.empty())
        return ROOTLESS_PATH_ERROR;

    if (p.front() != '/')
        return ROOTLESS_PATH_ERROR;
    
    if (!pathCset.has(p)) {
        return INVALID_PATH_ERROR;
    }

    this->path = p;
    return NO_ERROR;
}

auto URL::setQuery(std::string_view q) -> UrlError {
    if (!queryCset.has(q)) {
        return INVALID_QUERY_ERROR;
    }
    this->query = q;
    return NO_ERROR;
}

auto parseRequestURL(const std::string&) -> Expected<URL, UrlError> {
    return URL{};
}

}  // namepsace http
