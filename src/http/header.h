#pragma once

#include "src/util/string.h"
#include "writer.h"
#include "util/string.h"

#include <initializer_list>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <map>

namespace http {

class Headers {
 public:
    using ValueList = std::vector<std::string>;
    using InitializerList = std::initializer_list< std::pair<const std::string&, ValueList> >;
    using Map = std::map<std::string, ValueList, LessCaseInsesitive>;
    using ConstIterator = Map::const_iterator;

 private:
    Map     __header_map;

 public:
    void add(const std::string& key, const std::string& value) noexcept;
    void add(const std::string& key, const ValueList& value) noexcept;
    void add(InitializerList hdrs_list) noexcept;

    void set(const std::string& key, const std::string& value) noexcept;
    void set(const std::string& key, const ValueList& value) noexcept;
    void set(const std::string& key, ValueList&& value) noexcept;
    void set(InitializerList hdrs_list) noexcept;

    void erase(const std::string& key) noexcept;
    void clear() noexcept;

    size_t count(const std::string& key) const noexcept;

    ConstIterator get(const std::string& key) const noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;

    Headers(InitializerList hdrs_list) noexcept;

    Headers() noexcept = default;
    Headers(const Headers& hdrs) noexcept = default;
    Headers(Headers&& hdrs) noexcept = default;
    Headers& operator=(const Headers& hdrs) noexcept = default;
    Headers& operator=(Headers&& hdrs) noexcept = default;
};

std::string toString(const Headers& hdrs, const NewLineFormat nl_fmt = NewLineFormat::NL_FMT_LF) noexcept;

}  // namepsace http
