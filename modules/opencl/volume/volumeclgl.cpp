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
 * Main file authors: Daniel Jönsson, Peter Steneteg
 *
 *********************************************************************************/

#include <modules/opencl/volume/volumeclgl.h>
#include <modules/opencl/image/layerclresizer.h>
#include <inviwo/core/util/assertion.h>

namespace inviwo {

VolumeCLGL::VolumeCLGL(const DataFormatBase* format, const Texture3D* data)
    : VolumeCLBase(), VolumeRepresentation(data != NULL ? data->getDimension(): uvec3(64), format)
    , texture_(data) {

    if(data) {
        initialize(data);
    }
    
}

VolumeCLGL::VolumeCLGL(const uvec3& dimensions, const DataFormatBase* format, const Texture3D* data)
    : VolumeCLBase(), VolumeRepresentation(dimensions, format)
    , texture_(data){

    initialize(data);
}

VolumeCLGL::VolumeCLGL(const VolumeCLGL& rhs) 
    : VolumeRepresentation(rhs) {
    OpenCL::instance()->getQueue().enqueueCopyImage(*clImage_, 
                                                    get(),
                                                    glm::svec3(0), 
                                                    glm::svec3(0),
                                                    glm::svec3(dimensions_));

}

VolumeCLGL::~VolumeCLGL() { 
    deinitialize(); 
}

void VolumeCLGL::initialize(const Texture3D* texture) {
    ivwAssert(texture != 0, "Cannot initialize with null OpenGL texture");
    clImage_ = new cl::Image3DGL(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, GL_TEXTURE_3D, 0, texture->getID());
    
    VolumeCLGL::initialize();
}

VolumeCLGL* VolumeCLGL::clone() const {
    return new VolumeCLGL(*this);
}

void VolumeCLGL::deinitialize() {
   delete clImage_; // Delete OpenCL image before texture
}

void VolumeCLGL::notifyBeforeTextureInitialization() {
    delete clImage_; clImage_ = 0; 
}

void VolumeCLGL::notifyAfterTextureInitialization() {
    clImage_ = new cl::Image3DGL(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, GL_TEXTURE_3D, 0, texture_->getID());
}


} // namespace

namespace cl {

template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::VolumeCLGL& value)
{
    return setArg(index, value.get());
}


} // namespace cl
