#include "charset.h"
#include <string_view>

CharSet::CharSet(const char* s)
    : CharSet(std::string{s}) {
}

CharSet::CharSet(const std::string& s)
    : __set( {0} ) {
    for (auto c: s) {
        this->__set[ static_cast<unsigned>(c) ] = true;
    }
}

auto CharSet::has(const char c) const -> bool {
    return (*this)[int(c)];
}

auto CharSet::has(std::string_view str) const -> bool {
    for (auto c: str) {
        if (!this->has(c)) {
            return false;
        }
    }
    return true;
}

auto CharSet::set(const char c) -> CharSet& {
    unsigned char set_index = c;
    (*this)[set_index] = true;
    return *this;
}

auto CharSet::reset(const char c) -> CharSet& {
    unsigned char set_index = c;
    (*this)[set_index] = false;
    return *this;
}

auto CharSet::operator[](int c) const -> const bool& {
    return this->__set[c];
}

auto CharSet::operator[](int c) -> bool& {
    return this->__set[c];
}

auto CharSet::fill() -> CharSet& {
    for (auto& fl: this->__set)
        fl = true;
    return *this;
}

auto CharSet::clear() -> CharSet& {
    for (auto& fl: this->__set)
        fl = false;
    return *this;
}

auto CharSet::include(const CharSet& cset) -> CharSet& {
    for (size_t i = 0; i < CharSet::SET_SIZE; ++i) {
        (*this)[i] = cset[i] || (*this)[i];
    }
    return *this;
}

auto CharSet::exclude(const CharSet& cset) -> CharSet& {
    for (size_t i = 0; i < CharSet::SET_SIZE; ++i) {
        (*this)[i] = cset[i] ? false : (*this)[i];
    }
    return *this;
}

auto CharSet::include(const std::string& cset) -> CharSet& {
    for (auto c: cset)
        this->set(c);
    return *this;
}

auto CharSet::exclude(const std::string& cset) -> CharSet& {
    for (auto c: cset)
        this->reset(c);
    return *this;
}

auto operator-(const CharSet& lhs, const CharSet& rhs) -> CharSet {
    CharSet newcset;

    for (size_t i = 0; i < CharSet::SET_SIZE; ++i) {
        newcset[i] = rhs[i] ? false : lhs[i];
    }

    return newcset;
}

auto operator+(const CharSet& lhs, const CharSet& rhs) -> CharSet {
    CharSet newcset;

    for (size_t i = 0; i < CharSet::SET_SIZE; ++i) {
        newcset[i] = lhs[i] || rhs[i];
    }

    return newcset;
}

auto operator||(const CharSet& lhs, const CharSet& rhs) -> CharSet {
    return lhs + rhs;
}
