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

#include <modules/opencl/volume/volumecl.h>

namespace inviwo {

VolumeCL::VolumeCL(const DataFormatBase* format, const void* data)
    :  VolumeRepresentation(uvec3(128,128,128), format), imageFormat_(dataFormatToCLImageFormat(format->getId())), image3D_(0)
{
    initialize(data);
}

VolumeCL::VolumeCL(uvec3 dimensions, const DataFormatBase* format, const void* data)
    : image3D_(0), VolumeRepresentation(dimensions, format), imageFormat_(dataFormatToCLImageFormat(format->getId()))
{
    initialize(data);
}

VolumeCL::~VolumeCL() {
    deinitialize(); 
}

void VolumeCL::initialize(const void* voxels) {
    image3D_ = new cl::Image3D(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, getFormat(), dimensions_.x, dimensions_.y, dimensions_.z);
    if (voxels != NULL) {
        OpenCL::instance()->getQueue().enqueueWriteImage(*image3D_, true, glm::svec3(0), glm::svec3(dimensions_), 0, 0, const_cast<void*>(voxels));
    }
    VolumeCL::initialize();
}

DataRepresentation* VolumeCL::clone() const {
    VolumeCL* newVolumeCL = new VolumeCL(dimensions_, getDataFormat());
    OpenCL::instance()->getQueue().enqueueCopyImage(*image3D_, (newVolumeCL->getVolume()), glm::svec3(0), glm::svec3(0), glm::svec3(dimensions_));
    return newVolumeCL;
}

void VolumeCL::deinitialize() {
    delete image3D_;
    image3D_ = 0;
}
void VolumeCL::upload( const void* data )
{
    OpenCL::instance()->getQueue().enqueueWriteImage(*image3D_, true, glm::svec3(0), glm::svec3(dimensions_), 0, 0, const_cast<void*>(data));
}

void VolumeCL::download( void* data ) const
{
    OpenCL::instance()->getQueue().enqueueReadImage(*image3D_, true, glm::svec3(0), glm::svec3(dimensions_), 0, 0, data);
}

} // namespace

namespace cl {

template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::VolumeCL& value)
{
    return setArg(index, value.getVolume());
}


} // namespace cl
