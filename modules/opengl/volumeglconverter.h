#ifndef IVW_VOLUMEGLCONVERTER_H
#define IVW_VOLUMEGLCONVERTER_H

#include "inviwo/core/datastructures/representationconverter.h"
#include "inviwo/core/datastructures/volumeram.h"
#include "inviwo/core/datastructures/volumedisk.h"
#include "volumegl.h"

namespace inviwo {

    class VolumeGLConverter : public RepresentationConverterType<VolumeGL> {

    public:
        VolumeGLConverter();
        virtual ~VolumeGLConverter();

        inline bool canConvert(DataRepresentation* source) {
            if (!dynamic_cast<VolumeRAM*>(source)) {
                if (!dynamic_cast<VolumeDisk*>(source))
                    return false;
            }
            return true;
        }

        DataRepresentation* convert(DataRepresentation* source);
    };

} // namespace

#endif // IVW_VOLUMEGLCONVERTER_H
