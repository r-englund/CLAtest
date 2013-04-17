#ifndef IVW_SYSTEMINFO_H
#define IVW_SYSTEMINFO_H

#include <inviwo/core/common/inviwocoredefine.h>
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
            uint64_t mhz;
        };

        struct MemoryInfo {
            uint64_t total; //In Bytes
            uint64_t available; //In Bytes
        };

        struct DiskInfo {
            std::string diskName;
            std::string diskType;
            uint64_t total; //In Bytes
            uint64_t free; //In Bytes
        };

        struct ProcessMemoryInfo {
            uint64_t residentMem; //In Bytes
            uint64_t sharedMem; //In Bytes
            uint64_t virtualMem; //In Bytes
        };

        SystemInfo();
        virtual ~SystemInfo();

        void printInfo();
        
        bool canAllocate(uint64_t dataSize, uint8_t percentageOfAvailableMemory = 100);
        uvec3 calculateOptimalBrickSize(uvec3 dimensions, size_t formatSizeInBytes, uint8_t percentageOfAvailableMemory = 100);

        uint64_t getAvailableMemory();

    protected:
        void retrieveStaticInfo();
        void retrieveDynamicInfo();

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
        //bool successProcessMemoryInfo_;
    };

} // namespace

#endif // IVW_SYSTEMINFO_H
