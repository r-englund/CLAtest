#ifndef IVW_MESHGL_H
#define IVW_MESHGL_H

#include <modules/opengl/geometry/geometrygl.h>
#include <inviwo/core/datastructures/geometry/attributes.h>
#include <modules/opengl/geometry/attributebuffergl.h>
#include <inviwo/core/datastructures/geometry/meshram.h>
#include <inviwo/core/datastructures/representationconverter.h>
#include <vector>

namespace inviwo {

class IVW_MODULE_OPENGL_API MeshRenderer {

public:
    MeshRenderer(const Mesh* mesh);
    MeshRenderer(const Mesh* mesh, Mesh::AttributesInfo);
    MeshRenderer(const Mesh* mesh, Mesh::RenderType rt, Mesh::ConnectivityType ct);
    virtual ~MeshRenderer();

    virtual void render(Mesh::RenderType = Mesh::NOT_SPECIFIED) const;


    GLenum getDrawMode(Mesh::RenderType, Mesh::ConnectivityType);

protected:

    virtual void initialize(Mesh::AttributesInfo = Mesh::AttributesInfo());
    void initializeIndexBuffer( const Buffer* indexBuffer, Mesh::AttributesInfo ai );
    void renderArray(Mesh::RenderType) const;
    void renderElements(Mesh::RenderType) const;
    void emptyFunc(Mesh::RenderType rt) const{};

    typedef void (MeshRenderer::*DrawFunc)(Mesh::RenderType) const;
    struct DrawMethod{
        DrawFunc drawFunc;
        GLenum drawMode;
        const Buffer* elementBuffer;
    };

    DrawMethod drawMethods_[Mesh::NUMBER_OF_RENDER_TYPES];
    const Mesh* meshToRender_;
};

} // namespace

#endif // IVW_MESHGL_H
