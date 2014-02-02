/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/core/util/stringconversion.h>

namespace inviwo {

std::vector<std::string> splitString(const std::string& str, char delimeter) {
    std::vector<std::string> strings;
    std::stringstream stream(str);
    std::string part;

    while (std::getline(stream, part, delimeter))
        strings.push_back(part);

    return strings;
}

std::vector<std::string> splitStringWithMultipleDelimiters(const std::string& str, std::vector<char> delimiters) {
    if (!delimiters.size()) {
        //adding default delimiters
        delimiters.push_back('_');
        delimiters.push_back('+');
        delimiters.push_back('-');
        delimiters.push_back('.');
        delimiters.push_back(' ');
    }

    std::string tempString = str;
    char lastDelimiter = delimiters[delimiters.size()-1];

    for (size_t i=0; i<delimiters.size()-1; i++)
        replaceInString(tempString, toString(delimiters[i]), toString(lastDelimiter));

    return splitString(tempString, lastDelimiter);
}

std::string removeFromString(std::string str, char char_to_remove) {
    str.erase(std::remove(str.begin(), str.end(), char_to_remove), str.end());
    return str;
}

void replaceInString(std::string& str, const std::string& oldStr, const std::string& newStr) {
    size_t pos = 0;

    while ((pos = str.find(oldStr, pos)) != std::string::npos) {
        str.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
}

std::string parseTypeIdName(std::string str) {
    replaceInString(str, "class", "");
    replaceInString(str, "const", "");
    replaceInString(str, "inviwo::", "");
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64)
    replaceInString(str, "__ptr64", "");
#endif
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

unsigned int countLines(std::string str) {
    unsigned int lineCount = 1;
    size_t position = 0;

    while (position < str.length()) {
        if (str.substr(position,1) == "\n")
            lineCount++;

        position++;
    }

    return lineCount;
}

static const std::string possibleValues =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
std::string randomString(unsigned int length) {
    std::string s;

    while (s.size()<length)
        s += possibleValues[rand()%possibleValues.size()];

    return s;
}

} // namespace