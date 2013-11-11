#ifndef IVW_BUFFERGL_H
#define IVW_BUFFERGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>
#include <inviwo/core/datastructures/geometry/attributes.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API BufferGL: public BufferRepresentation {

public:
    BufferGL(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage);
    virtual ~BufferGL();

    virtual void initialize();
    virtual void deinitialize();
    virtual BufferGL* clone() const;

    const Buffer* getAttribute() const;
    GLenum getFormatType() const;
    GLuint getId() const;
    GLFormats::GLFormat getGLFormat() const { return glFormat_; }

    void enable() const;
    void disable() const;

    void bind() const;
    void specifyLocation() const;

    void upload(const void* data, size_t size, GLenum target = GL_ARRAY_BUFFER);
    void reupload(const void* data, size_t size);

    void download(void* data) const;

protected:
    void enableArray() const;
    void disableArray() const;


    void colorPointer() const;
    void normalPointer() const;
    void texCoordPointer() const;
    void vertexPointer() const;

    void emptyFunc() const;

private:
    GLuint id_;
    GLenum state_;
    GLenum usageGL_;
    GLenum target_;
    GLFormats::GLFormat glFormat_;
    void (BufferGL::*locationPointerFunc_)() const;

};



} // namespace

#endif // IVW_BUFFERGL_H
