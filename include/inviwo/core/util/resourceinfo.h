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

        virtual void retrieveStaticInfo() = 0;
        virtual void retrieveDynamicInfo() = 0;
        virtual void printInfo() = 0;

    protected:
        static const std::string logSource_; ///< Source string to be displayed for log messages.
    };

} // namespace

#endif // IVW_RESOURCEINFO_H
