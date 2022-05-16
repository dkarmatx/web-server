#include <gtest/gtest.h>
#include <string>

#include "http/url.h"

TEST(Http_Url_SetUserinfo, ValidUserinfo) {
    auto tester = [](const std::string& usrinfo) {
        http::URL       url;
        http::UrlError  error = url.setUserinfo(usrinfo);

        ASSERT_EQ(error, http::NO_ERROR);
        ASSERT_EQ(url.userinfo, usrinfo);
    };

    tester("usrname:password");
    tester("usrname_here_is_the_some");
    tester("SoNiceToHaveAnUser");
    tester("HerePctEncoding%6D%6D");
}

TEST(Http_Url_SetUserinfo, InvalidUserinfo) {
    const std::string   initial_usrinfo = "xxx";
    http::URL           url;

    url.userinfo = initial_usrinfo;

    auto tester = [&url, &initial_usrinfo] (const std::string& usrinfo) {
        http::UrlError  error = url.setUserinfo(usrinfo);

        ASSERT_EQ(error, http::INVALID_USERINFO_ERROR);
        ASSERT_EQ(url.userinfo, initial_usrinfo);
    };

    tester("b@dUserInf0");
    tester("hello it is invalid");
    tester("ads/bads/abads");
    tester("lol?here");
}

TEST(Http_Url_SetUserinfo, EmptyUserinfo) {
    http::URL          url;

    url.userinfo = "yyyy";

    http::UrlError     error = url.setUserinfo("");

    ASSERT_EQ(error, http::NO_ERROR);
    ASSERT_EQ(url.userinfo, "");
}
