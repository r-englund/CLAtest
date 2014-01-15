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

#include <modules/opencl/volume/volumeclgl.h>
#include <modules/opencl/image/layerclresizer.h>
#include <inviwo/core/util/assertion.h>

namespace inviwo {

VolumeCLGL::VolumeCLGL(const DataFormatBase* format, const Texture3D* data)
    : VolumeRepresentation(data != NULL ? data->getDimension(): uvec3(64), format)
    , image3D_(0)
    , texture_(data) {

    if(data) {
        initialize(data);
    }
    
}

VolumeCLGL::VolumeCLGL(const uvec3& dimensions, const DataFormatBase* format, const Texture3D* data)
    : VolumeRepresentation(dimensions, format)
    , texture_(data){

    initialize(data);
}

VolumeCLGL::VolumeCLGL(const VolumeCLGL& rhs) 
    : VolumeRepresentation(rhs) {
    OpenCL::instance()->getQueue().enqueueCopyImage(*image3D_, 
                                                    getVolume(),
                                                    glm::svec3(0), 
                                                    glm::svec3(0),
                                                    glm::svec3(dimensions_));

}

VolumeCLGL::~VolumeCLGL() { 
    deinitialize(); 
}

void VolumeCLGL::initialize(const Texture3D* texture) {
    ivwAssert(texture != 0, "Cannot initialize with null OpenGL texture");
    image3D_ = new cl::Image3DGL(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, GL_TEXTURE_3D, 0, texture->getID());
    VolumeCLGL::initialize();
}

VolumeCLGL* VolumeCLGL::clone() const {
    return new VolumeCLGL(*this);
}

void VolumeCLGL::deinitialize() {
    delete image3D_;
    image3D_ = 0; 
}

} // namespace

namespace cl {

template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::VolumeCLGL& value)
{
    return setArg(index, value.getVolume());
}


} // namespace cl
