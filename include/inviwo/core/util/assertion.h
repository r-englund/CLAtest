#ifndef IVW_ASSERTION_H
#define IVW_ASSERTION_H

#include <iostream>

#if defined(IVW_DEBUG)
    #if defined(WIN32)
        #define IVW_BREAKPOINT __debugbreak()
    #else
        #define IVW_BREAKPOINT
    #endif
#else
    #define IVW_BREAKPOINT
#endif

void ivwAssertion(const char* fileName, const char* functionName, long lineNumber, const char* message);

#if defined(IVW_DEBUG)
#define ivwAssert(condition, message) \
    if (!(bool(condition))) ivwAssertion(__FILE__, __FUNCTION__, __LINE__, (message));
#else
#define ivwAssert(condition, message)
#endif

#endif // IVW_ASSERTION_H
