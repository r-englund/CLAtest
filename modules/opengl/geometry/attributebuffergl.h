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
    GLenum getFormatType() const;

    void enable() const;
    void disable() const;

    void bind() const;
    void specifyLocation() const;
    void upload(GLenum usage = GL_STATIC_DRAW);

protected:
    void enableArray() const;
    void disableArray() const;

    void enableElementArray() const;

    void colorPointer() const;
    void normalPointer() const;
    void texCoordPointer() const;
    void vertexPointer() const;

    void emptyFunc() const;

private:
    const AttributesBase* attrib_;
    GLuint id_;
    GLenum state_;
    GLenum target_;
    GLFormats::GLFormat glFormat_;
    void (AttributeBufferGL::*locationPointerFunc_)() const;
    void (AttributeBufferGL::*enableFunc_)() const;
    void (AttributeBufferGL::*disableFunc_)() const;

};

} // namespace

#endif // IVW_ATTRIBUTEBUFFERGL_H
