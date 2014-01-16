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

#ifndef IVW_TEXTURE2D_H
#define IVW_TEXTURE2D_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/glwrap/texture.h>
#include <inviwo/core/util/observer.h>
#include <inviwo/core/util/referencecounter.h>
#include <modules/opengl/textureobserver.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API Texture2D : public Texture, public Observable<TextureObserver>, public ReferenceCounter {

public:
    Texture2D(uvec2 dimensions, GLFormats::GLFormat glFormat, GLenum filtering);
    Texture2D(uvec2 dimensions, GLint format, GLint internalformat, GLenum dataType, GLenum filtering);
    Texture2D(const Texture2D& other);
    Texture2D& operator=(const Texture2D& other);
    virtual ~Texture2D();

    Texture2D* clone() const;

    void initialize(const void* data);

    size_t getNumberOfValues() const;

    void upload(const void* data);

    const uvec2& getDimension() const { return dimensions_;}
    int getWidth() const{ return dimensions_.x; }
    int getHeight() const{ return dimensions_.y; }
    void resize(uvec2 dimension);

protected:
    void setWidth(int x) { dimensions_.x = x; }
    void setHeight(int y) { dimensions_.y = y; }

private:
    uvec2 dimensions_;
};

} // namespace

#endif // IVW_TEXTURE2D_H
