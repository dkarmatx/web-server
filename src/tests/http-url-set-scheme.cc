#include <gtest/gtest.h>

#include "http/url.h"
#include "util/string.h"

TEST(Http_Url_SetScheme, ValidScheme) {
    auto tester = [](const std::string& s) {
        http::URL      url;
        http::UrlError error = url.setScheme(s);

        ASSERT_EQ(error, http::NO_ERROR);
        ASSERT_EQ(url.scheme, toLower(s));
    };

    tester("http");
    tester("hTTp");
    tester("eXtra+.-");
    tester("proto3");
}

TEST(Http_Url_SetScheme, InvalidSchemeSymbol) {
    const std::string empty_scheme = "";

    auto tester = [&empty_scheme] (const std::string& test_str) {
        http::URL      url;
        http::UrlError error;

        error = url.setScheme(test_str);
        ASSERT_EQ(error, http::INVALID_SCHEME_ERROR);
        ASSERT_EQ(url.scheme, empty_scheme);
    };

    tester("http:");
    tester("http=");
    tester("http%6D");
    tester("http  ");
    tester("  http");
    tester("3proto");
}

TEST(Http_Url_SetScheme, EmptyScheme) {
    http::URL      url;
    http::UrlError error;

    error = url.setScheme("");
    ASSERT_EQ(error, http::EMPTY_SCHEME_ERROR);
    ASSERT_EQ(url.scheme, std::string{""});
}
