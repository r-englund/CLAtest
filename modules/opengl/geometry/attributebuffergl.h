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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

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

    void initialize(const void* data, GLsizeiptr sizeInBytes, GLenum target = GL_ARRAY_BUFFER);
    void upload(const void* data, GLsizeiptr sizeInBytes);

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
