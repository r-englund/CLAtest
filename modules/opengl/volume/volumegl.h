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
    VolumeGL(uvec3 dimensions = uvec3(128,128,128), const DataFormatBase* format = DataFormatBase::get(), Texture3D* tex = NULL);
    VolumeGL(const VolumeGL& rhs);
    VolumeGL& operator=(const VolumeGL& rhs);
    virtual ~VolumeGL();
    virtual VolumeGL* clone() const;

    virtual std::string getClassName() const { return "VolumeGL"; }
 
    void bindTexture(GLenum texUnit) const;
    void unbindTexture() const;

    void setDimension(uvec3 dimensions);

    Texture3D* getTexture();
    const Texture3D* getTexture() const;

protected:
    void initialize();
    void deinitialize();

private:
    Texture3D* volumeTexture_;
};

} // namespace

#endif // IVW_VOLUMEGL_H
