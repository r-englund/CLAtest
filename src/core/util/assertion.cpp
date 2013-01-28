#include <inviwo/core/util/assertion.h>
#include <inviwo/core/inviwo.h>

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