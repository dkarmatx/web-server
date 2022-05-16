#include <gtest/gtest.h>

#include "http/url.h"

TEST(Http_Url_SetOpaque, ValidOpaque) {
    auto tester = [](const std::string& opaque) {
        http::URL       url;
        http::UrlError  error = url.setOpaque(opaque);

        ASSERT_EQ(url.opaque, opaque);
        ASSERT_EQ(error, http::NO_ERROR);
    };

    tester("hello");
    tester("highload:some:here");
    tester("hello@tosomeone.com");
    tester("strange%0Fthings");
    tester("strange/very/much");
    tester("strange.hello.by.high");
}

TEST(Http_Url_SetOpaque, InvalidOpaque) {
    http::URL         url;
    const std::string initial_opqaue = "aaa";

    url.opaque = initial_opqaue;

    auto tester = [&initial_opqaue, &url](const std::string& opaque) {
        http::UrlError  error = url.setOpaque(opaque);

        ASSERT_EQ(url.opaque, initial_opqaue);
        ASSERT_EQ(error, http::INVALID_OPAQUE_ERROR);
    };

    tester("hello sd");
    tester("??HiImaHere");
    tester("bro?");
    tester("      ");
}

TEST(Http_Url_SetOpaque, EmptyOpaque) {
    http::URL         url;

    url.opaque = "xxxxxx";

    http::UrlError  error = url.setOpaque("");

    ASSERT_EQ(url.opaque, "");
    ASSERT_EQ(error, http::NO_ERROR);
}
