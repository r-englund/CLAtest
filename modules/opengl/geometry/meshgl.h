#ifndef IVW_MESHGL_H
#define IVW_MESHGL_H

#include <modules/opengl/geometry/geometrygl.h>
#include <inviwo/core/datastructures/geometry/attributes.h>
#include <vector>

namespace inviwo {

class IVW_MODULE_OPENGL_API MeshGL : public GeometryGL {

public:
    MeshGL();
    virtual ~MeshGL();
    virtual void performOperation(DataOperation*) const = 0;
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const = 0;
    virtual void render(RenderType = NOT_SPECIFIED) = 0;

    void createNewBuffer(AttributesBase*);

protected:
    std::vector<GLuint> vertexBufferIds_;
    std::vector<GLuint> faceBufferIds_;
};

} // namespace

#endif // IVW_MESHGL_H
