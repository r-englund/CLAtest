#include <inviwo/core/util/exception.h>

namespace inviwo {

Exception::Exception(const std::string& message = "")
    : message_(message) {}

const std::string Exception::getMessage() const throw() {
    return message_;
};

} // namespace
