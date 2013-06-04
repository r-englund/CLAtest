#ifndef IVW_ASSERTION_H
#define IVW_ASSERTION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <iostream>

IVW_CORE_API void ivwAssertion(const char* fileName, const char* functionName, long lineNumber, std::string message);

#if defined(IVW_DEBUG)
#define ivwAssert(condition, message) \
{   std::ostringstream stream__; stream__ << message; \
    IVW_CORE_API if (!(bool(condition))) ivwAssertion(__FILE__, __FUNCTION__, __LINE__, (stream__.str()));}
#else
#define ivwAssert(condition, message)
#endif

#endif // IVW_ASSERTION_H
