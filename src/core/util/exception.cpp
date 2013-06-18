#include <inviwo/core/util/exception.h>

namespace inviwo {

Exception::Exception(const std::string& message)
    : message_(message) {}

const std::string Exception::getMessage() const throw() {
    return message_;
};

IgnoreException::IgnoreException(const std::string& message)
: Exception(message) {}

AbortException::AbortException(const std::string& message)
: Exception(message) {}

SerializationException::SerializationException(const std::string& message)
: Exception(message) {}

} // namespace
