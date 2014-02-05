 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file author: Daniel Jönsson
 *
 *********************************************************************************/

#ifndef IVW_VOLUMECLGL_H
#define IVW_VOLUMECLGL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>
#include <modules/opencl/volume/volumeclbase.h>
#include <modules/opengl/glwrap/texture3d.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API VolumeCLGL: public VolumeCLBase, public VolumeRepresentation, public TextureObserver {

public:
    VolumeCLGL(const DataFormatBase* format = DataFormatBase::get(), const Texture3D* data = NULL);
    VolumeCLGL(const uvec3& dimensions, const DataFormatBase* format, const Texture3D* data);
    VolumeCLGL(const VolumeCLGL& rhs);

    virtual VolumeCLGL* clone() const;
    virtual ~VolumeCLGL();
    virtual std::string getClassName() const { return "VolumeCLGL"; }
    virtual void initialize(){};
    virtual void deinitialize();
    
    
    void initialize(const Texture3D* texture);
    virtual void setDimension(uvec3 dimensions) { dimensions_ = dimensions; deinitialize(); initialize(texture_); }

    virtual cl::Image3D& getEditable() { return *static_cast<cl::Image3D*>(clImage_); }
    virtual const cl::Image3D& get() const { return *const_cast<const cl::Image3D*>(static_cast<const cl::Image3D*>(clImage_)); }
    const Texture3D* getTexture() const { return texture_; }

    /**
    * This method will be called before the texture is initialized.
    * Override it to add behavior.
    */
    virtual void notifyBeforeTextureInitialization();

    /**
    * This method will be called after the texture has been initialized.
    * Override it to add behavior.
    */
    virtual void notifyAfterTextureInitialization();

    void aquireGLObject(std::vector<cl::Event>* syncEvents = NULL, const cl::CommandQueue& queue = OpenCL::instance()->getQueue()) const {
        std::vector<cl::Memory> syncImages(1, *clImage_); 
        queue.enqueueAcquireGLObjects(&syncImages, syncEvents);
    }
    void releaseGLObject(std::vector<cl::Event>* syncEvents = NULL, cl::Event* event= NULL, const cl::CommandQueue& queue = OpenCL::instance()->getQueue()) const {
        std::vector<cl::Memory> syncImages(1, *clImage_); 
        queue.enqueueReleaseGLObjects(&syncImages, syncEvents, event);
    }

protected:
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
