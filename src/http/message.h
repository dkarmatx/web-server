#pragma once

#include "common.h"
#include "header.h"
#include <string>

namespace http {

enum class Method {
    METHOD_UNDEFINED = 0,
    METHOD_GET,
    METHOD_POST,
    METHOD_DELETE,
    METHOD_HEAD,
    METHOD_CONNECT  //TODO: forward proxies
};

namespace __internal {

class MessageBase {
 private:
    Headers         __hdrs;
    std::string     __body;

 public:
    Headers& headers() noexcept;
    const Headers& headers() const noexcept;

    std::string& body() noexcept;
    const std::string& body() const noexcept;

    MessageBase(const MessageBase& msgb) noexcept = default;
    MessageBase(MessageBase&& msgb) noexcept = default;
    MessageBase& operator=(const MessageBase& msgb) noexcept = default;
    MessageBase& operator=(MessageBase&& msgb) noexcept = default;

    MessageBase(std::string&& body = std::string(), Headers&& hdrs = Headers()) noexcept
        : __hdrs(std::move(hdrs))
        , __body(std::move(body)) {
    }

    MessageBase(const std::string& body = std::string(), const Headers& hdrs = Headers()) noexcept
        : __hdrs(hdrs)
        , __body(body) {
    }
};

}  // namespace http::__internal

class Request : public __internal::MessageBase {

};

class Response : public __internal::MessageBase {

};

}  // namepsace http
