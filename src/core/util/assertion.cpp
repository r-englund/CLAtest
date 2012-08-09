#include "inviwo/core/util/assertion.h"

#if defined(IVW_DEBUG)

void ivwAssertion(const char* fileName, const char* functionName, long lineNumber, const char* message) {
    std::cout << "Assertion (" << fileName << ", " << functionName << ", Ln " << lineNumber << "): " << std::endl;
    std::cout << "    " << message << std::endl;
    std::cout << "    activate (b)reakpoint, (i)gnore, (e)xit" << std::endl;
    while (true) {
        std::string keyboardInput = "";
        std::cin >> keyboardInput;
        switch (keyboardInput[0]) {
            case 'b':
                IVW_BREAKPOINT;
                break;
            case 'i':
                return;
            case 'e':
                exit(0);
        }
    }
}

#endif // IVW_DEBUG