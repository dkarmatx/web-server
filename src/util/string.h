#pragma once

#include <string>
#include <string_view>


std::string  toUpper(const std::string& s) noexcept;
std::string  toUpper(const std::string_view s) noexcept;

std::string  toLower(const std::string& s) noexcept;
std::string  toLower(const std::string_view s) noexcept;

struct LessCaseInsesitive {
    bool operator()(const std::string& l, const std::string& r) const noexcept;
};
