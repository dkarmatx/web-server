#pragma once

#include <string>
#include <string_view>

class CharSet {
 public:
    constexpr static size_t     SET_SIZE = 256;

    CharSet() noexcept = default;
    CharSet(const CharSet&) noexcept = default;
    CharSet(CharSet&&) noexcept = default;

    CharSet(const char* str) noexcept;
    CharSet(const std::string& str) noexcept;

    bool        has(const char c) const noexcept;
    bool        has(std::string_view str) const noexcept;
    const bool& operator[](int c) const noexcept;
    bool&       operator[](int c) noexcept;

 private:
    bool __set[SET_SIZE] = { 0 };
};

CharSet     operator+(const CharSet& lhs, const CharSet& rhs) noexcept;
CharSet     operator||(const CharSet& lhs, const CharSet& rhs) noexcept;
