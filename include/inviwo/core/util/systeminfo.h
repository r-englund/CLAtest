#ifndef IVW_SYSTEMINFO_H
#define IVW_SYSTEMINFO_H

#include <inviwo/core/inviwocoredefine.h>
#include <sstream>

namespace inviwo {

IVW_CORE_API size_t getTotalRAM();

IVW_CORE_API void getSystemInfo(std::ostringstream&);

IVW_CORE_API void printSystemInfo();

} // namespace

#endif // IVW_SYSTEMINFO_H
