#ifndef IVW_VOLUMEDISK_H
#define IVW_VOLUMEDISK_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/datastructures/volumerepresentation.h>
#include <stdio.h>
#include <fstream>

namespace inviwo {

    class IVW_CORE_API VolumeDisk : public VolumeRepresentation {

    public:
        VolumeDisk(std::string url);
        virtual ~VolumeDisk();
        virtual void initialize();
        virtual void deinitialize();
        virtual DataRepresentation* clone();
        virtual std::string getSourceFile() {return sourceFile_;}

        void* loadRawData();

    private:
        std::string sourceFile_;
    };

} // namespace

#endif // IVW_VOLUMEDISK_H
