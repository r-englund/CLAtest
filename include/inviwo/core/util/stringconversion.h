#ifndef IVW_STRINGCONVERSION_H
#define IVW_STRINGCONVERSION_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <string>
#include <sstream>

namespace inviwo {

    template<class T>
    IVW_CORE_API std::string toString(T value) {
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }

    template<class T>
    IVW_CORE_API T stringTo(const std::string& str) {
        T result;
        std::istringstream stream;
        stream.str(str);
        stream >> result;
        return result;
    }

    IVW_CORE_API std::vector<std::string> splitString(const std::string& str, char delimeter = ' ') {
        std::vector<std::string> strings;
        std::stringstream stream(str);
        std::string part;
        while(std::getline(stream, part, delimeter)) {
            strings.push_back(part);
        }
        return strings;
    }

} // namespace

#endif // IVW_STRINGCONVERSION_H
