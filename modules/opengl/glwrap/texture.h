/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
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

#ifndef IVW_TEXTURE_H
#define IVW_TEXTURE_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API Texture {

public:
    Texture(GLenum, GLFormats::GLFormat glFormat, GLenum filtering);
    Texture(GLenum, GLint format, GLint internalformat, GLenum dataType, GLenum filtering);
    Texture(const Texture& other);
    Texture& operator=(const Texture& other);
    virtual ~Texture();

    virtual Texture* clone() const = 0;

    virtual size_t getNumberOfValues() const = 0;
    
    virtual void upload(const void* data) = 0;

    GLuint getID() const;

    GLuint getNChannels() const;
    GLuint getSizeInBytes() const;

    GLenum getFormat() const;
    GLenum getDataType() const;

    void bind() const;
    void unbind() const;

    void download(void* data) const;
    
    void loadFromPBO(const Texture*);

protected:
    void bindFromPBO() const;
    void bindToPBO() const;    
    void unbindFromPBO() const;
    void unbindToPBO() const;

    void downloadToPBO() const;

    void invalidatePBO();

    void setupAsyncReadBackPBO();

    void setNChannels();
    void setSizeInBytes();

    GLenum target_;
    GLenum format_;
    GLenum internalformat_;
    GLenum dataType_;
    GLenum filtering_;

private:
    GLuint id_;
    GLuint pboBack_; //For asynchronous readback to CPU 

    GLuint byteSize_;
    GLuint numChannels_;

    mutable bool dataInReadBackPBO_;
};

} // namespace

#endif // IVW_TEXTURE_H
