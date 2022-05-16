#include "charset.h"
#include <string_view>

CharSet::CharSet(const char* s) noexcept
    : CharSet(std::string{s})
{
}

CharSet::CharSet(const std::string& s) noexcept {
    for (auto c: s) {
        this->__set[ static_cast<unsigned>(c) ] = true;
    }
}

auto CharSet::has(const char c) const noexcept -> bool {
    return (*this)[int(c)];
}

auto CharSet::has(std::string_view str) const noexcept -> bool {
    for (auto c: str) {
        if (!this->has(c)) {
            return false;
        }
    }
    return true;
}

auto CharSet::operator[](int c) const noexcept -> const bool& {
    return this->__set[c];
}

auto CharSet::operator[](int c) noexcept -> bool& {
    return this->__set[c];
}

auto operator+(const CharSet& lhs, const CharSet& rhs) noexcept -> CharSet {
    return lhs || rhs;
}

auto operator||(const CharSet& lhs, const CharSet& rhs) noexcept -> CharSet {
    CharSet newcset;

    for (size_t i = 0; i < CharSet::SET_SIZE; ++i) {
        newcset[i] = lhs[i] || rhs[i];
    }

    return newcset;
}