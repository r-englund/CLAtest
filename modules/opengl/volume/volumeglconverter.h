#ifndef IVW_VOLUMEGLCONVERTER_H
#define IVW_VOLUMEGLCONVERTER_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramconverter.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>
#include "volumegl.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API VolumeRAM2GLConverter : public RepresentationConverterType<VolumeGL> {

public:
    VolumeRAM2GLConverter();
    virtual ~VolumeRAM2GLConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const VolumeRAM*>(source) != NULL;
    }

    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENGL_API VolumeGL2RAMConverter : public RepresentationConverterType<VolumeRAM> {

public:
    VolumeGL2RAMConverter();
    virtual ~VolumeGL2RAMConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const VolumeGL*>(source) != NULL;
    }

    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
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
