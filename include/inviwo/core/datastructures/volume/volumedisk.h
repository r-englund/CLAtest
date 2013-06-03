#ifndef IVW_VOLUMEDISK_H
#define IVW_VOLUMEDISK_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/diskrepresentation.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>
#include <stdio.h>
#include <fstream>

namespace inviwo {

    class IVW_CORE_API VolumeDisk : public VolumeRepresentation, public DiskRepresentation {

    public:
        VolumeDisk();
        VolumeDisk(std::string url);
        virtual ~VolumeDisk();
        virtual void initialize();
        virtual void deinitialize();
        virtual DataRepresentation* clone() const;
        void* loadRawData();
    };

} // namespace

#endif // IVW_VOLUMEDISK_H
