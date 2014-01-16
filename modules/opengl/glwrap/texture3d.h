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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_TEXTURE3D_H
#define IVW_TEXTURE3D_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/glwrap/texture.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API Texture3D : public Texture {

public:
    Texture3D(uvec3 dimensions, GLFormats::GLFormat glFormat, GLenum filtering);
    Texture3D(uvec3 dimensions, GLint format, GLint internalformat, GLenum dataType, GLenum filtering);
    Texture3D(const Texture3D& other);
    Texture3D& operator=(const Texture3D& other);
    virtual ~Texture3D();

    Texture3D* clone() const;

    void initialize(const void* data);

    size_t getNumberOfValues() const;

    void upload(const void* data);

    const uvec3& getDimension() const { return dimensions_; }

private:
    uvec3 dimensions_;
};

} // namespace

#endif // IVW_TEXTURE3D_H
