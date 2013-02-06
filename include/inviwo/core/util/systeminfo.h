#ifndef IVW_SYSTEMINFO_H
#define IVW_SYSTEMINFO_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/util/resourceinfo.h>
#include <vector>
#include <string>

namespace inviwo {

    class IVW_CORE_API SystemInfo : public ResourceInfo {

    public:
        struct OSInfo {
            std::string description;
            int platform;
        };

        struct CPUInfo {
            std::string vendor;
            std::string model;
            size_t mhz;
        };

        struct MemoryInfo {
            size_t total; //MB
            size_t available; //MB
        };

        struct DiskInfo {
            std::string diskName;
            std::string diskType;
            size_t total; //MB
            size_t free; //MB
        };

        struct ProcessMemoryInfo {
            size_t residentMem; //MB
            size_t sharedMem; //MB
            size_t virtualMem; //MB
        };

        SystemInfo();
        virtual ~SystemInfo();

        void printInfo();

        size_t getAvailableMemory();

    protected:
        void retrieveStaticInfo();
        void retrieveDynamicInfo();

        static const std::string logSource_; ///< Source string to be displayed for log messages.

    private:
        bool lookupOSInfo();
        bool lookupCPUInfo();
        bool lookupMemoryInfo();
        bool lookupDiskInfo();
        bool lookupProcessMemoryInfo();

        OSInfo infoOS_;
        std::vector<CPUInfo> infoCPUs_;
        MemoryInfo infoRAM_;
        std::vector<DiskInfo> infoDisks_;
        ProcessMemoryInfo infoProcRAM_;

        bool successOSInfo_;
        bool successCPUInfo_;
        bool successMemoryInfo_;
        bool successDiskInfo_;
        bool successProcessMemoryInfo_;
    };

} // namespace

#endif // IVW_SYSTEMINFO_H
