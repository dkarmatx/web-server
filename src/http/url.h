#pragma once

#include "util/expected.h"
#include <string>
#include <string_view>

namespace http {

using UrlError = const char *;

constexpr UrlError NO_ERROR                 = nullptr;
constexpr UrlError MISSING_SCHEME_ERROR     = "missing porotocol scheme";
constexpr UrlError ROOTLESS_PATH_ERROR      = "rootles path is forbidden in request URI";

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
    std::string     fragment;

    bool operator==(const URL& rhs) const noexcept;

    UrlError setScheme(std::string_view scheme) noexcept;

    UrlError setOpaque(std::string_view opaque) noexcept;

    UrlError setUserinfo(std::string_view usrinfo) noexcept;

    UrlError setHost(std::string_view host) noexcept;

    UrlError setPath(std::string_view path) noexcept;

    UrlError setQuery(std::string_view query) noexcept;

    UrlError setFragment(std::string_view fragment) noexcept;
};

Expected<URL, UrlError> parseRequestURL(const std::string& raw_url) noexcept;

}  // namespace http
