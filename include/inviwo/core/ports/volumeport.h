#ifndef IVW_VOLUMEPORT_H
#define IVW_VOLUMEPORT_H

#include "inviwo/core/ports/dataport.h"
#include "inviwo/core/datastructures/volume.h"

namespace inviwo {

    class VolumePort : public DataPort<Volume> {

    public:
        VolumePort(Port::PortDirection direction, std::string identifier);
        virtual ~VolumePort();

        void initialize();
        void deinitialize();

        virtual ivec3 getColorCode() { return ivec3(188,101,101); }
    };

} // namespace

#endif // IVW_VOLUMEPORT_H
