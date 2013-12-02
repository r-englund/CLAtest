/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include <inviwo/core/util/assertion.h>
#include <inviwo/core/common/inviwo.h>

#include <sstream>

#if defined(IVW_DEBUG)

void ivwAssertion(const char* fileName, const char* functionName, long lineNumber, std::string message) {
    std::cout << "Assertion in (" << fileName << ", " << functionName << ", Ln " << lineNumber << "): ";
    std::cout << message << std::endl;
    std::cout << "(choose to ";
    #if defined(IVW_DEBUG) && defined(WIN32)
        std::cout << "(b)reak, ";
    #endif
    std::cout << "(i)gnore or (e)xit):";
    while (true) {
        std::string keyboardInput = "";
        std::cin >> keyboardInput;
        switch (keyboardInput[0]) {
            #if defined(IVW_DEBUG) && defined(WIN32)
                case 'b': __debugbreak();
            #endif
                case 'i': return;
                case 'e': exit(0);
        }
    }
}

#endif // IVW_DEBUG