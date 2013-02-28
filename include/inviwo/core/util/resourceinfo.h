#ifndef IVW_RESOURCEINFO_H
#define IVW_RESOURCEINFO_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>

namespace inviwo {

    class IVW_CORE_API ResourceInfo {

    public:
        ResourceInfo(){}
        virtual ~ResourceInfo(){}

        virtual void initialize(){
            retrieveStaticInfo();
        }

        virtual void printInfo() = 0;

        virtual bool canAllocate(uint64_t dataSize, uint8_t percentageOfAvailableMemory){
            return true;
        }
    
        virtual uvec3 calculateOptimalBrickSize(uvec3 dimensions, size_t formatSizeInBytes, uint8_t percentageOfAvailableMemory){
            return dimensions;
        }

    protected:
        virtual uint64_t getMemorySizeInBytes(uvec3 dimensions, size_t formatSizeInBytes){
            return static_cast<uint64_t>(dimensions.x*dimensions.y*dimensions.z*formatSizeInBytes);
        }

        virtual void retrieveStaticInfo() = 0;
        virtual void retrieveDynamicInfo() = 0;
    };

} // namespace

#endif // IVW_RESOURCEINFO_H
