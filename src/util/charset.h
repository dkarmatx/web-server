#pragma once

#include <string>
#include <array>
#include <string_view>

class CharSet {
 public:
    constexpr static size_t     SET_SIZE = 256;

    CharSet(const CharSet&) = default;
    CharSet(CharSet&&) = default;

    CharSet(const char* str);
    CharSet(const std::string& str = std::string{});

    bool        has(const char c) const;
    bool        has(std::string_view str) const;

    CharSet&    set(const char c);
    CharSet&    reset(const char c);

    const bool& operator[](int c) const;
    bool&       operator[](int c);

    CharSet& fill();
    CharSet& clear();
    CharSet& exclude(const CharSet& cset);
    CharSet& exclude(const std::string& str_set);
    CharSet& include(const CharSet& cset);
    CharSet& include(const std::string& str_set);

 private:
    std::array<bool, SET_SIZE> __set;
};

CharSet     operator-(const CharSet& lhs, const CharSet& rhs);
CharSet     operator+(const CharSet& lhs, const CharSet& rhs);
CharSet     operator||(const CharSet& lhs, const CharSet& rhs);
