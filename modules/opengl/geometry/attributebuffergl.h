#ifndef IVW_BUFFERGL_H
#define IVW_BUFFERGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>
#include <inviwo/core/datastructures/geometry/attributes.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API BufferGL: public BufferRepresentation {

public:
    BufferGL(size_t size, BufferType type, const DataFormatBase* format);
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

    void upload(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW, GLenum target = GL_ARRAY_BUFFER);
    void reupload(const void* data, size_t size ,GLenum usage = GL_STATIC_DRAW);

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
    GLenum target_;
    GLFormats::GLFormat glFormat_;
    void (BufferGL::*locationPointerFunc_)() const;

};

class IVW_MODULE_OPENGL_API BufferRAM2GLConverter : public RepresentationConverterType<BufferGL> {

public:
    BufferRAM2GLConverter();
    virtual ~BufferRAM2GLConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const BufferRAM*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};


} // namespace

#endif // IVW_BUFFERGL_H
