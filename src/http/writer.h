#pragma once

#include <string>

namespace http {

enum class NewLineFormat {
    NL_FMT_CRLF,
    NL_FMT_LF
};

std::string toString(NewLineFormat nl);

}
