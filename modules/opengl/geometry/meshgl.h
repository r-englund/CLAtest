#ifndef IVW_MESHGL_H
#define IVW_MESHGL_H

#include <modules/opengl/geometry/geometrygl.h>
#include <inviwo/core/datastructures/geometry/attributes.h>
#include <modules/opengl/geometry/attributebuffergl.h>
#include <inviwo/core/datastructures/geometry/meshram.h>
#include <inviwo/core/datastructures/representationconverter.h>
#include <vector>

namespace inviwo {

class IVW_MODULE_OPENGL_API MeshGL : public GeometryGL {

public:
    MeshGL();
    MeshGL(MeshRAM::AttributesInfo);
    virtual ~MeshGL();
    virtual void performOperation(DataOperation*) const {};
    virtual void initialize(MeshRAM::AttributesInfo);
    virtual void deinitialize();
    virtual DataRepresentation* clone() const { return NULL; };
    virtual void render(RenderType = GeometryRepresentation::NOT_SPECIFIED) const;

    //Create and add Vertex Buffer with data
    void createArrayBuffer(const AttributesBase*);

    //Create and add Element Array Buffer for rendering using indices
    void createElementBuffer(const AttributesBase*, MeshRAM::AttributesInfo);

    GLenum getDrawMode(RenderType, ConnectivityType);

    const AttributeBufferGL* getArrayBufferGL(size_t) const;

protected:
    void renderArray(RenderType) const;
    void renderElements(RenderType) const;
    void emptyFunc(RenderType rt) const{};

    std::vector<AttributeBufferGL*> arrayBuffers_;
    
    typedef void (MeshGL::*DrawFunc)(RenderType) const;
    struct DrawMethod{
        DrawFunc drawFunc;
        GLenum drawMode;
        AttributeBufferGL* elementBuffer;
    };

    DrawMethod drawMethods_[GeometryRepresentation::NUMBER_OF_RENDER_TYPES];
};

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

#endif // IVW_MESHGL_H
