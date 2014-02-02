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
std::string toString(T value) {
    std::ostringstream stream;
    stream << value;
    return stream.str();
}

template<class T>
T stringTo(const std::string& str) {
    T result;
    std::istringstream stream;
    stream.str(str);
    stream >> result;
    return result;
}

IVW_CORE_API std::vector<std::string> splitString(const std::string& str, char delimeter = ' ');
IVW_CORE_API std::vector<std::string> splitStringWithMultipleDelimiters(const std::string& str,
        std::vector<char> delimiters=std::vector<char>());

IVW_CORE_API std::string removeFromString(std::string str, char char_to_remove = ' ');
IVW_CORE_API void replaceInString(std::string& str, const std::string& oldStr, const std::string& newStr);
IVW_CORE_API std::string parseTypeIdName(std::string str) ;

IVW_CORE_API std::string toUpper(std::string str);
IVW_CORE_API std::string toLower(std::string str);

IVW_CORE_API unsigned int countLines(std::string str);

IVW_CORE_API std::string randomString(unsigned int length = 10);

} // namespace

#endif // IVW_STRINGCONVERSION_H
