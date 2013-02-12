#ifndef IVW_STRINGCONVERSION_H
#define IVW_STRINGCONVERSION_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

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

    IVW_CORE_API std::string removeFromString(std::string str, char char_to_remove = ' ') {
        str.erase(std::remove(str.begin(), str.end(), char_to_remove), str.end());
        return str;
    }

    IVW_CORE_API std::string toUpper(std::string str) {
        std::transform(str.begin(), str.end(),str.begin(), ::toupper);
        return str;
    }

    IVW_CORE_API std::string toLower(std::string str) {
        std::transform(str.begin(), str.end(),str.begin(), ::tolower);
        return str;
    }

} // namespace

#endif // IVW_STRINGCONVERSION_H
