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

#ifndef IVW_VOLUMEGL_H
#define IVW_VOLUMEGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>
#include <modules/opengl/glwrap/texture3d.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API VolumeGL : public VolumeRepresentation {

public:
    VolumeGL(uvec3 dimensions = uvec3(128,128,128), const DataFormatBase* format = DataFormatBase::get());
    VolumeGL(uvec3 dimensions, GLint format, GLint internalFormat, GLenum dataType);
    VolumeGL(const void* texels, uvec3 dimensions = uvec3(128,128,128), const DataFormatBase* format = DataFormatBase::get());
    VolumeGL(const void* texels, uvec3 dimensions, GLint format, GLint internalFormat, GLenum dataType);
    virtual ~VolumeGL();

    virtual void initialize();
    virtual void initialize(const void* texels);
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;
    GLint getFormat();
    GLint getInternalFormat();
    GLenum getDataType();
 
    virtual void bindTexture(GLenum texUnit) const;
    virtual void unbindTexture() const;

    void upload(const void* data);
    void download(void* data) const;

    const Texture3D* getTexture() const { return volumeTexture_; }

protected:
    GLint format_;
    GLint internalFormat_;
    GLenum dataType_;
    Texture3D* volumeTexture_;
};

} // namespace

#endif // IVW_VOLUMEGL_H
