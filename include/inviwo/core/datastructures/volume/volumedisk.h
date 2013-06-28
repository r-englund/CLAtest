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
        VolumeDisk(uvec3 dimensions = uvec3(128,128,128), DataFormatBase format = DataUINT8());
        VolumeDisk(std::string url, uvec3 dimensions = uvec3(128,128,128), DataFormatBase format = DataUINT8());
        virtual ~VolumeDisk();
        virtual void initialize();
        virtual void deinitialize();
        virtual DataRepresentation* clone() const;
        void* loadRawData() const;
    };

} // namespace

#endif // IVW_VOLUMEDISK_H
