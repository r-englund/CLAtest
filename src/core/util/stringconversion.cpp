#include <inviwo/core/util/stringconversion.h>

namespace inviwo {

std::vector<std::string> splitString(const std::string& str, char delimeter) {
    std::vector<std::string> strings;
    std::stringstream stream(str);
    std::string part;
    while(std::getline(stream, part, delimeter)) {
        strings.push_back(part);
    }
    return strings;
}

std::string removeFromString(std::string str, char char_to_remove) {
    str.erase(std::remove(str.begin(), str.end(), char_to_remove), str.end());
    return str;
}

void replaceInString(std::string& str, const std::string& oldStr, const std::string& newStr) {
    size_t pos = 0;
    while((pos = str.find(oldStr, pos)) != std::string::npos){
        str.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
}

std::string parseTypeIdName(std::string str) {
    replaceInString(str, "class", "");
    replaceInString(str, "const", "");
    replaceInString(str, "inviwo::", "");
    return removeFromString(removeFromString(str, '*'), ' ');
}

std::string toUpper(std::string str) {
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    return str;
}

std::string toLower(std::string str) {
    std::transform(str.begin(), str.end(),str.begin(), ::tolower);
    return str;
}

} // namespace