/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_ELEMENT_BUFFER_GL_H
#define IVW_ELEMENT_BUFFER_GL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>
#include <inviwo/core/datastructures/buffer/bufferram.h>

namespace inviwo {


class IVW_MODULE_OPENGL_API ElementBufferGL: public BufferRepresentation {

public:
    ElementBufferGL(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage);
    virtual ~ElementBufferGL();

    virtual void initialize();
    virtual void deinitialize();
    virtual ElementBufferGL* clone() const;

    const Buffer* getAttribute() const;
    GLenum getFormatType() const;
    GLuint getId() const;
    GLFormats::GLFormat getGLFormat() const { return glFormat_; }

    void enable() const;
    void disable() const;

    void bind() const;
    void specifyLocation() const;

    void upload(const void* data, size_t size);
    void reupload(const void* data, size_t size);

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
    GLenum usageGL_;
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
