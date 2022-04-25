#include "string.h"

#include <algorithm>
#include <cctype>
#include <string_view>

auto isAlpha(const int c) noexcept -> bool {
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z');
}

auto isNum(const int c) noexcept -> bool {
    return '0' <= c && c <= '9';
}

auto toUpper(const std::string& s) noexcept -> std::string {
    std::string res(s);

    for (auto& c : res) {
        c = std::toupper(c);
    }

    return res;
}

auto toUpper(const std::string_view s) noexcept -> std::string {
    std::string res(s);

    for (auto& c : res) {
        c = std::toupper(c);
    }

    return res;
}

auto toLower(const std::string& s) noexcept -> std::string {
    std::string res(s);

    for (auto& c : res) {
        c = std::tolower(c);
    }

    return res;
}

auto toLower(const std::string_view s) noexcept -> std::string {
    std::string res(s);

    for (auto& c : res) {
        c = std::tolower(c);
    }

    return res;
}

auto LessCaseInsesitive::operator()(const std::string &l, const std::string &r) const noexcept -> bool {
    return toUpper(l) < toUpper(r);
}
