#ifndef IVW_ATTRIBUTEBUFFERGL_H
#define IVW_ATTRIBUTEBUFFERGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/datastructures/geometry/attributes.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API AttributeBufferGL {

public:
    AttributeBufferGL(const AttributesBase*, GLenum target = GL_ARRAY_BUFFER, bool element = false);
    virtual ~AttributeBufferGL();
    const AttributesBase* getAttribute() const;
    GLenum getFormatType();

    void enable();
    void disable();

    void bind();
    void specifyLocation();
    void upload(GLenum usage = GL_STATIC_DRAW);

protected:
    void enableArray();
    void disableArray();

    void enableElementArray();

    void colorPointer();
    void normalPointer();
    void texCoordPointer();
    void vertexPointer();

    void emptyFunc();

private:
    const AttributesBase* attrib_;
    GLuint id_;
    GLenum state_;
    GLenum target_;
    GLFormats::GLFormat glFormat_;
    void (AttributeBufferGL::*locationPointerFunc_)();
    void (AttributeBufferGL::*enableFunc_)();
    void (AttributeBufferGL::*disableFunc_)();

};

} // namespace

#endif // IVW_ATTRIBUTEBUFFERGL_H
