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

#ifndef IVW_VOLUMECLGL_H
#define IVW_VOLUMECLGL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>
#include <modules/opengl/glwrap/texture3d.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API VolumeCLGL : public VolumeRepresentation {

public:
    VolumeCLGL(const DataFormatBase* format = DataFormatBase::get(), const Texture3D* data = NULL);
    VolumeCLGL(const uvec3& dimensions, const DataFormatBase* format, const Texture3D* data);
    virtual ~VolumeCLGL();
    virtual std::string getClassName() const { return "VolumeCLGL"; }
    virtual void initialize(){};
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;
    
    void initialize(const Texture3D* texture);
    virtual void setDimension(uvec3 dimensions) { dimensions_ = dimensions; deinitialize(); initialize(texture_); }

    const cl::Image3DGL& getVolume() const { return *(image3D_); }
    const Texture3D* getTexture() const { return texture_; }

    void aquireGLObject(std::vector<cl::Event>* syncEvents = NULL, const cl::CommandQueue& queue = OpenCL::instance()->getQueue()) const {
        std::vector<cl::Memory> syncImages(1, *image3D_); 
        queue.enqueueAcquireGLObjects(&syncImages, syncEvents);
    }
    void releaseGLObject(std::vector<cl::Event>* syncEvents = NULL, cl::Event* event= NULL, const cl::CommandQueue& queue = OpenCL::instance()->getQueue()) const {
        std::vector<cl::Memory> syncImages(1, *image3D_); 
        queue.enqueueReleaseGLObjects(&syncImages, syncEvents, event);
    }

protected:
    cl::Image3DGL* image3D_;
    const Texture3D* texture_;
};

} // namespace

namespace cl {

// Kernel argument specializations for VolumeCLGL type 
// (enables calling cl::Queue::setArg with VolumeCLGL)
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const inviwo::VolumeCLGL& value);

} // namespace cl



#endif // IVW_VOLUMECLGL_H
