#ifndef IVW_STRINGCONVERSION_H
#define IVW_STRINGCONVERSION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
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

    IVW_CORE_API std::vector<std::string> splitString(const std::string& str, char delimeter = ' ') ;
    IVW_CORE_API std::string removeFromString(std::string str, char char_to_remove = ' ');

    IVW_CORE_API void replaceInString(std::string& str, const std::string& oldStr, const std::string& newStr);
    IVW_CORE_API std::string parseTypeIdName(std::string str) ;

    IVW_CORE_API std::string toUpper(std::string str);
    IVW_CORE_API std::string toLower(std::string str);

} // namespace

#endif // IVW_STRINGCONVERSION_H
