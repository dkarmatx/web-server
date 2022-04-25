#include "message.h"
#include "http/header.h"
#include <string>

namespace http::__internal {

auto MessageBase::headers() noexcept -> Headers& {
    return this->__hdrs;
}

auto MessageBase::headers() const noexcept -> const Headers& {
    return this->__hdrs;
}

auto MessageBase::body() noexcept -> std::string& {
    return this->__body;
}

auto MessageBase::body() const noexcept -> const std::string& {
    return this->__body;
}

}  // namespace http::__internal


namespace http {

}  // namespace http
