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

#include "texture2d.h"

namespace inviwo {

Texture2D::Texture2D(uvec2 dimensions, GLFormats::GLFormat glFormat, GLenum filtering, GLint level)
    : Texture(GL_TEXTURE_2D, glFormat, filtering, level), Observable<TextureObserver>(), ReferenceCounter()
    , dimensions_(dimensions) 
{
    setTextureParameterFunction(this, &Texture2D::default2DTextureParameterFunction);
}

Texture2D::Texture2D(uvec2 dimensions, GLint format, GLint internalformat, GLenum dataType, GLenum filtering, GLint level)
    : Texture(GL_TEXTURE_2D, format, internalformat, dataType, filtering, level), Observable<TextureObserver>(), ReferenceCounter()
    , dimensions_(dimensions) 
{
    setTextureParameterFunction(this, &Texture2D::default2DTextureParameterFunction);
}

Texture2D::Texture2D(const Texture2D& rhs)
    : Texture(rhs), Observable<TextureObserver>(), ReferenceCounter()
    , dimensions_(rhs.dimensions_) 
{
    setTextureParameterFunction(this, &Texture2D::default2DTextureParameterFunction);
    initialize(NULL);   
    // TODO: Copy texture from other
    // bind();
    // glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, dimensions_.x, dimensions_.y);
}

Texture2D& Texture2D::operator=(const Texture2D& rhs) {
    if (this != &rhs) {
        Texture::operator=(rhs);
        // Check if this object is shared with OpenCL/CUDA/DirectX
        if(getRefCount() > 1) {
            LogError("This object is shared and cannot changed (size/format etc.) until the shared object has been released");
        }
        dimensions_ = rhs.dimensions_;
        setTextureParameterFunction(this, &Texture2D::default2DTextureParameterFunction);
        initialize(NULL);
        // TODO: Copy other texture content
    }
    return *this;
}

Texture2D::~Texture2D() {}

Texture2D* Texture2D::clone() const {
    return new Texture2D(*this);
}

void Texture2D::initialize(const void* data) {
    // Notify observers
    ObserverSet::iterator endIt = observers_->end();
    for(ObserverSet::iterator it = observers_->begin(); it != endIt; ++it) {
        // static_cast can be used since only template class objects can be added
        static_cast<TextureObserver*>(*it)->notifyBeforeTextureInitialization();    
    }
    bind();
    texParameterCallback_->invoke(this);
    glTexImage2D(GL_TEXTURE_2D, level_, internalformat_, dimensions_.x, dimensions_.y, 0, format_, dataType_, data);
    LGL_ERROR;
    for(ObserverSet::iterator it = observers_->begin(); it != endIt; ++it) {
        // static_cast can be used since only template class objects can be added
        static_cast<TextureObserver*>(*it)->notifyAfterTextureInitialization();    
    }
}

size_t Texture2D::getNumberOfValues() const{
    return static_cast<size_t>(dimensions_.x*dimensions_.y);
}

void Texture2D::upload(const void* data ) {
    bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, dimensions_.x, dimensions_.y, format_, dataType_, data);
    LGL_ERROR;
}

void Texture2D::resize(uvec2 dimension) {
    setWidth(dimension.x);
    setHeight(dimension.y);
    initialize(NULL);
}

void Texture2D::default2DTextureParameterFunction(Texture* tex){
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex->getFiltering());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex->getFiltering());
}

} // namespace
