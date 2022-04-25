#include "writer.h"

namespace http {

std::string toString(NewLineFormat nl) {
    switch (nl) {
        case NewLineFormat::NL_FMT_CRLF:    return "\r\n";

        case NewLineFormat::NL_FMT_LF:
        default:                            return "\n";
    }
}

}  // namespace http
