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
    virtual ~MeshGL();
    virtual void performOperation(DataOperation*) const {};
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const { return NULL; };
    virtual void render();

    void createArrayBuffer(const AttributesBase*);
    void createElementBuffer(const AttributesBase*);

    void setRenderer(bool element = true);

protected:
    void renderArray();
    void renderElements();
    void emptyFunc(){};

    std::vector<AttributeBufferGL*> arrayBuffers_;
    AttributeBufferGL* elementBuffer_;
    void (MeshGL::*drawFunc)();
};

class IVW_MODULE_OPENGL_API MeshRAM2GLConverter : public RepresentationConverterType<GeometryGL> {

public:
    MeshRAM2GLConverter();
    virtual ~MeshRAM2GLConverter();
    inline bool canConvert(DataRepresentation* source) {
        if (!dynamic_cast<MeshRAM*>(source)) {
            return false;
        }
        return true;
    }
    DataRepresentation* convert(DataRepresentation* source);
};

} // namespace

#endif // IVW_MESHGL_H
