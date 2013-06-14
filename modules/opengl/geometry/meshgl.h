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
    virtual void render() const;

    void createArrayBuffer(const AttributesBase*);
    void createElementBuffer(const AttributesBase*, Connectivity);

    void setRenderer(bool element = true);

protected:
    void renderArray() const;
    void renderElements() const;
    void emptyFunc() const{};

    std::vector<AttributeBufferGL*> arrayBuffers_;
    std::vector<std::pair<Connectivity, AttributeBufferGL*>> elementBuffers_;
    AttributeBufferGL* defaultElementBuffer_;
    GLenum elementDrawMode_;
    void (MeshGL::*drawFunc)() const;
};

class IVW_MODULE_OPENGL_API MeshRAM2GLConverter : public RepresentationConverterType<GeometryGL> {

public:
    MeshRAM2GLConverter();
    virtual ~MeshRAM2GLConverter();
    inline bool canConvert(const DataRepresentation* source) {
        if (!dynamic_cast<const MeshRAM*>(source)) {
            return false;
        }
        return true;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

} // namespace

#endif // IVW_MESHGL_H
