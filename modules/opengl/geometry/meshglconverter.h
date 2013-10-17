#ifndef IVW_MESHGLCONVERTER_H
#define IVW_MESHGLCONVERTER_H

#include <inviwo/core/datastructures/representationconverter.h>
#include <modules/opengl/geometry/meshgl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API MeshRAM2GLConverter : public RepresentationConverterType<GeometryGL> {

public:
    MeshRAM2GLConverter();
    virtual ~MeshRAM2GLConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const MeshRAM*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

} // namespace

#endif // IVW_MESHGLCONVERTER_H
