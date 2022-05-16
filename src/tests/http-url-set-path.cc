#include <gtest/gtest.h>

#include "http/url.h"

TEST(Http_Url_SetPath, ValidPath) {
    auto tester = [](const std::string& path) {
        http::URL       url;
        http::UrlError  error = url.setPath(path);

        ASSERT_EQ(error, http::NO_ERROR);
        ASSERT_EQ(url.path, path);
    };

    tester("/");
    tester("/normal/path/here");
    tester("//////wtf///wtf//");
    tester("//%2D%2D%2D");
}

TEST(Http_Url_SetPath, InvalidPath) {
    const std::string initial_path = "/hello";
    http::URL         url;

    url.path = initial_path;

    auto tester = [&url, &initial_path](const std::string& path) {
        http::UrlError  error = url.setPath(path);

        ASSERT_EQ(error, http::INVALID_PATH_ERROR);
        ASSERT_EQ(url.path, initial_path);
    };

    tester("/here i am");
    tester("/not????ausualsymbol");
    tester("/asdasd#nofrag");
}

TEST(Http_Url_SetPath, RootlessPath) {
    const std::string initial_path = "/hello";
    http::URL         url;

    url.path = initial_path;

    auto tester = [&url, &initial_path](const std::string& path) {
        http::UrlError  error = url.setPath(path);

        ASSERT_EQ(error, http::ROOTLESS_PATH_ERROR);
        ASSERT_EQ(url.path, initial_path);
    };

    tester("there/are/no/rootless/pathes");
    tester("");
    tester("asdsa");
}