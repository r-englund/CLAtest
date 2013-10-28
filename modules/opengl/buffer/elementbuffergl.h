#ifndef IVW_ELEMENT_BUFFER_GL_H
#define IVW_ELEMENT_BUFFER_GL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>
#include <inviwo/core/datastructures/buffer/bufferram.h>

namespace inviwo {


class IVW_MODULE_OPENGL_API ElementBufferGL: public BufferRepresentation {

public:
    ElementBufferGL(size_t size, BufferType type, const DataFormatBase* format);
    virtual ~ElementBufferGL();

    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;

    const Buffer* getAttribute() const;
    GLenum getFormatType() const;
    GLuint getId() const;
    GLFormats::GLFormat getGLFormat() const { return glFormat_; }

    void enable() const;
    void disable() const;

    void bind() const;
    void specifyLocation() const;

    void upload(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW);

    void update(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW);

protected:

    void enableElementArray() const;

    void colorPointer() const;
    void normalPointer() const;
    void texCoordPointer() const;
    void vertexPointer() const;

    void emptyFunc() const;

private:
    const Buffer* attrib_;
    GLuint id_;
    GLenum state_;
    GLFormats::GLFormat glFormat_;
    void (ElementBufferGL::*locationPointerFunc_)() const;
    void (ElementBufferGL::*enableFunc_)() const;
    void (ElementBufferGL::*disableFunc_)() const;

};


class IVW_MODULE_OPENGL_API BufferRAM2ElementGLConverter : public RepresentationConverterType<ElementBufferGL> {

public:
    BufferRAM2ElementGLConverter();
    virtual ~BufferRAM2ElementGLConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const BufferRAM*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};


} // namespace

#endif // IVW_ELEMENT_BUFFER_GL_H
