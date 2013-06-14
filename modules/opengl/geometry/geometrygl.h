#ifndef IVW_GEOMETRYGL_H
#define IVW_GEOMETRYGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/datastructures/geometry/geometryrepresentation.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API GeometryGL : public GeometryRepresentation {

public:
    GeometryGL();
    virtual ~GeometryGL();
    virtual void performOperation(DataOperation*) const = 0;
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const = 0;
    virtual void render() const = 0;
};

} // namespace

#endif // IVW_GEOMETRYGL_H
