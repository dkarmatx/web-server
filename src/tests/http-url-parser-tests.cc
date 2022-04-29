#include <gtest/gtest.h>

#include "http/url.h"
#include "util/expected.h"

TEST(HttpUrlParserTest, ValidAbsoluteUrl) {
    using namespace http;

    URL expected;
    expected.scheme = "https";
    expected.opaque = "";
    expected.userinfo = "user:password";
    expected.host = "42.school.fr:1000";
    expected.path = "/to/the/infinity";
    expected.query = "a=b&c=d&m";
    expected.fragment = "HelloGuys";

    auto r = parseRequestURL("https://user:password@42.school.fr:1000/to/the/infinity?a=b&c=d&m#HelloGuys");
    ASSERT_TRUE(r.isOk());
    ASSERT_EQ(expected, r.value());
}

TEST(HttpUrlParserTest, ValidOpaqueUrl) {
    using namespace http;

    URL expected;
    expected.scheme = "mailto";
    expected.opaque = "hgranule@student.21-pool.ru";
    expected.userinfo = "";
    expected.host = "";
    expected.path = "";
    expected.query = "theme=bye";
    expected.fragment = "frag";

    auto r = parseRequestURL("mailto:hgranule@student.21-pool.ru?theme=bye#frag");
    ASSERT_TRUE(r.isOk());
    ASSERT_EQ(expected, r.value());
}

TEST(HttpUrlParserTest, ValidAbsolutePath) {
    using namespace http;

    URL expected;
    expected.scheme = "";
    expected.opaque = "";
    expected.userinfo = "";
    expected.host = "";
    expected.path = "/here/is/an/absolute/path";
    expected.query = "";
    expected.fragment = "";

    auto r = parseRequestURL("/here/is/an/absolute/path");
    ASSERT_TRUE(r.isOk());
    ASSERT_EQ(expected, r.value());
}

TEST(HttpUrlParserTest, ValidAbsoluteUrlWithoutScheme) {
    using namespace http;

    URL expected;
    expected.scheme = "";
    expected.opaque = "";
    expected.userinfo = "";
    expected.host = "here-is-the-host.com";
    expected.path = "/here/is/an/absolute/path";
    expected.query = "";
    expected.fragment = "";

    auto r = parseRequestURL("//here-is-the-host.com/here/is/an/absolute/path");
    ASSERT_TRUE(r.isOk());
    ASSERT_EQ(expected, r.value());
}

TEST(HttpUrlParserTest, RelativePath) {
    using namespace http;

    auto r = parseRequestURL("rootless/path/is/invalid");
    ASSERT_TRUE(r.isError());
    ASSERT_EQ(r.error(), ROOTLESS_PATH_ERROR);
}

TEST(HttpUrlParserTest, EmptyScheme) {
    using namespace http;

    auto r = parseRequestURL(":hello.com/hey_there#frag");
    ASSERT_TRUE(r.isError());
    ASSERT_EQ(r.error(), EMPTY_SCHEME_ERROR);
}
