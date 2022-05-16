#include <gtest/gtest.h>

#include "http/url.h"

TEST(Http_Url_SetQuery, ValidQuery) {
    auto tester = [](const std::string& query) {
        http::URL       url;
        http::UrlError  error = url.setQuery(query);

        ASSERT_EQ(url.query, query);
        ASSERT_EQ(error, http::NO_ERROR);
    };

    tester("a=b&a=c");
    tester("a[]=b&a[]=c");
    tester("asdasd");
    tester("");
    tester("???asdsd");
}

TEST(Http_Url_SetQuery, InvalidQuery) {
    const std::string initial_query = "a&b=3&b=4&c=3";
    http::URL         url;

    url.query = initial_query;

    auto tester = [&url, &initial_query](const std::string& query) {
        http::UrlError  error = url.setQuery(query);

        ASSERT_EQ(url.query, initial_query);
        ASSERT_EQ(error, http::INVALID_QUERY_ERROR);
    };

    tester("str with spaces");
    tester("query++with[]fuck");
    tester("lol###");
}