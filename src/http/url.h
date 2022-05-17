#pragma once

#include "util/expected.h"
#include <string>
#include <string_view>

namespace http {

using UrlError = const char *;

constexpr UrlError NO_ERROR                 = nullptr;
constexpr UrlError EMPTY_SCHEME_ERROR       = "a";
constexpr UrlError INVALID_SCHEME_ERROR     = "b";
constexpr UrlError INVALID_OPAQUE_ERROR     = "e";
constexpr UrlError INVALID_USERINFO_ERROR   = "g";
constexpr UrlError ROOTLESS_PATH_ERROR      = "h";
constexpr UrlError INVALID_PATH_ERROR       = "c";
constexpr UrlError INVALID_QUERY_ERROR      = "j";
constexpr UrlError EMPTY_PORT_ERROR         = "i";
constexpr UrlError INVALID_PORT_ERROR       = "k";
constexpr UrlError INVALID_HOSTNAME_ERROR   = "d";

/**
 *  Structure which contains all URL parts.
 *  Fields MUST be encoded.
 */
struct URL {
    std::string     scheme;
    std::string     opaque;
    std::string     userinfo;
    std::string     host;
    std::string     path;
    std::string     query;
    std::string     fragment;  // unused usually

    bool operator==(const URL& rhs) const;

    UrlError setScheme(std::string_view scheme);
    UrlError setOpaque(std::string_view opaque);
    UrlError setUserinfo(std::string_view usrinfo);
    UrlError setHost(std::string_view host);
    UrlError setPath(std::string_view path);
    UrlError setQuery(std::string_view query);
};

Expected<URL, UrlError> parseRequestURL(const std::string& raw_url);

}  // namespace http
