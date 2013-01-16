#ifndef IVW_VOLUMEGLCONVERTER_H
#define IVW_VOLUMEGLCONVERTER_H

#include "modules/opengl/openglmoduledefine.h"
#include "inviwo/core/datastructures/representationconverter.h"
#include "inviwo/core/datastructures/volumeram.h"
#include "inviwo/core/datastructures/volumeramconverter.h"
#include "inviwo/core/datastructures/volumedisk.h"
#include "volumegl.h"

namespace inviwo {

    class IVW_MODULE_OPENGL_API VolumeRAM2GLConverter : public RepresentationConverterType<VolumeGL> {

    public:
        VolumeRAM2GLConverter();
        virtual ~VolumeRAM2GLConverter();

        inline bool canConvert(DataRepresentation* source) {
            if (!dynamic_cast<VolumeRAM*>(source)) {
                return false;
            }
            return true;
        }

        DataRepresentation* convert(DataRepresentation* source);
    };

    class IVW_MODULE_OPENGL_API VolumeDisk2GLConverter : public RepresentationConverterPackage<VolumeGL> {
    
    public:
        VolumeDisk2GLConverter() : RepresentationConverterPackage<VolumeGL>(){
            addConverter(new VolumeDisk2RAMConverter());
            addConverter(new VolumeRAM2GLConverter());
        };
        virtual ~VolumeDisk2GLConverter() {};
    };



} // namespace

#endif // IVW_VOLUMEGLCONVERTER_H
