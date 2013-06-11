#ifndef IVW_TRIANGLEMESHGL_H
#define IVW_TRIANGLEMESHGL_H

#include <modules/opengl/geometry/meshgl.h>
#include <inviwo/core/datastructures/geometry/trianglemeshram.h>
#include <inviwo/core/datastructures/representationconverter.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API TriangleMeshGL : public MeshGL {

public:
    TriangleMeshGL();
    virtual ~TriangleMeshGL();
    virtual void performOperation(DataOperation*) const {};
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const { return NULL; };
    virtual void render(RenderType = NOT_SPECIFIED);
};

class IVW_MODULE_OPENGL_API TriangleMeshRAM2GLConverter : public RepresentationConverterType<GeometryGL> {

public:
    TriangleMeshRAM2GLConverter();
    virtual ~TriangleMeshRAM2GLConverter();
    inline bool canConvert(DataRepresentation* source) {
        if (!dynamic_cast<TriangleMeshRAM*>(source)) {
            return false;
        }
        return true;
    }
    DataRepresentation* convert(DataRepresentation* source);
};

} // namespace

#endif // IVW_TRIANGLEMESHGL_H
