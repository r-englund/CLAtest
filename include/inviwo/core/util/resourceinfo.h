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

    protected:
        virtual void retrieveStaticInfo() = 0;
        virtual void retrieveDynamicInfo() = 0;
    };

} // namespace

#endif // IVW_RESOURCEINFO_H
