#include "header.h"
#include "src/http/writer.h"
#include <sstream>
#include <utility>
#include <vector>

namespace http {

Headers::Headers(Headers::InitializerList hdrs_list) noexcept {
    for (const auto& [key, values] : hdrs_list) {
        this->__header_map.emplace(std::make_pair(key, values));
    }
}

auto Headers::add(const std::string& key, const std::string& value) noexcept -> void {
    auto it = this->__header_map.find(key);

    if (it == this->end()) {
        this->__header_map.emplace(std::make_pair(key, ValueList{value}));
    } else {
        it->second.push_back(value);
    }
}

auto Headers::add(const std::string& key, const ValueList& values) noexcept -> void {
    auto it = this->__header_map.find(key);

    if (it == this->end()) {
        this->__header_map.emplace(std::make_pair(key, values));
    } else {
        auto& old_vals = it->second;
        old_vals.reserve(old_vals.size() + values.size());
        for (const auto& val : values) {
            old_vals.emplace_back(val);
        }
    }
}

auto Headers::add(Headers::InitializerList hdrs_list) noexcept -> void {
    for (const auto& [key, vls] : hdrs_list) {
        this->add(key, vls);
    }
}

auto Headers::set(const std::string& key, const std::string& value) noexcept -> void {
    this->set(key, ValueList{value});
}

auto Headers::set(const std::string& key, const ValueList& value) noexcept -> void {
    this->__header_map[key] = value;
}

auto Headers::set(const std::string& key, ValueList&& value) noexcept -> void {
    this->__header_map[key] = std::move(value);
}

auto Headers::set(InitializerList hdrs_list) noexcept -> void {
    for (auto [key, values] : hdrs_list) {
        this->set(key, values);
    }
}

auto Headers::erase(const std::string& key) noexcept -> void {
    this->__header_map.erase(key);
}

auto Headers::clear() noexcept -> void {
    this->__header_map.clear();
}

auto Headers::count(const std::string& key) const noexcept -> size_t {
    auto it = this->__header_map.find(key);

    if (it == this->end()) {
        return 0;
    } else {
        return it->second.size();
    }
}

auto Headers::get(const std::string& key) const noexcept -> ConstIterator {
    return this->__header_map.find(key);
}

auto Headers::begin() const noexcept -> ConstIterator {
    return this->__header_map.begin();
}

auto Headers::end() const noexcept -> ConstIterator {
    return this->__header_map.end();
}

auto toString(const Headers& hdrs, NewLineFormat nl_fmt) noexcept -> std::string {
    const std::string nl{toString(nl_fmt)};

    std::stringstream ss;
    for (const auto& [hdr_key, hdr_value_list] : hdrs) {
        for (const auto& hdr_value : hdr_value_list) {
            ss << hdr_key << ": " << hdr_value << nl;
        }
    }

    return ss.str();
}

}  // namepsace http
