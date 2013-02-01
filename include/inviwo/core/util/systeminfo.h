#ifndef IVW_SYSTEMINFO_H
#define IVW_SYSTEMINFO_H

#include <inviwo/core/inviwocoredefine.h>
#include <vector>
#include <string>

namespace inviwo {

struct IVW_CORE_API OSInfo {
    std::string description;
    int platform;
};

struct IVW_CORE_API CPUInfo {
    std::string vendor;
    std::string model;
    size_t mhz;
};

struct IVW_CORE_API MemoryInfo {
    size_t total; //MB
    size_t available; //MB
};

struct IVW_CORE_API DiskInfo {
    std::string diskName;
    std::string diskType;
    size_t total; //MB
    size_t free; //MB
};

struct IVW_CORE_API ProcessMemoryInfo {
    size_t residentMem; //MB
    size_t sharedMem; //MB
    size_t virtualMem; //MB
};

IVW_CORE_API bool lookupOSInfo(OSInfo&);
IVW_CORE_API bool lookupCPUInfo(std::vector<CPUInfo>&);
IVW_CORE_API bool lookupMemoryInfo(MemoryInfo&);
IVW_CORE_API bool lookupDiskInfo(std::vector<DiskInfo>&);
IVW_CORE_API bool lookupProcessMemoryInfo(ProcessMemoryInfo&);

IVW_CORE_API void printSystemInfo();

} // namespace

#endif // IVW_SYSTEMINFO_H
