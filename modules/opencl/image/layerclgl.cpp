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
 * Main file authors: Daniel Jönsson, Erik Sundén
 *
 *********************************************************************************/

#include <modules/opencl/image/layerclgl.h>
#include <modules/opencl/image/layerclresizer.h>
#include <inviwo/core/util/assertion.h>

namespace inviwo {

LayerCLGL::LayerCLGL(uvec2 dimensions, LayerType type, const DataFormatBase* format, Texture2D* data)
    : LayerRepresentation(dimensions, type, format), texture_(data)
{
    if(data) {
        initialize(data);
    }
}

LayerCLGL::~LayerCLGL() { 
    deinitialize(); 
}

void LayerCLGL::initialize(Texture2D* texture) {
    ivwAssert(texture != 0, "Cannot initialize with null OpenGL texture");
    // Indicate that the texture should not be deleted.
    texture->increaseRefCount();
    clImage_ = new cl::Image2DGL(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, texture->getID());
    texture->addObserver(this);
    LayerCLGL::initialize();
}

LayerCLGL* LayerCLGL::clone() const {
    Texture2D* clonedTexture = new Texture2D(*texture_); 
    LayerCLGL* newLayerCLGL = new LayerCLGL(dimensions_, getLayerType(), getDataFormat(), clonedTexture);
    return newLayerCLGL;
}

void LayerCLGL::deinitialize() {
    delete clImage_; // Delete OpenCL image before texture
    if(texture_ && texture_->decreaseRefCount() <= 0){
        delete texture_;
        texture_ = NULL;
    }
}


void LayerCLGL::resize(uvec2 dimensions) {
    if (dimensions == dimensions_) {
        return;
    }
    // Make sure that the OpenCL layer is deleted before resizing the texture
    // By observing the texture we will make sure that the OpenCL layer is 
    // deleted and reattached after resizing is done.
    const_cast<Texture2D*>(texture_)->resize(dimensions);
    LayerRepresentation::resize(dimensions);
}

bool LayerCLGL::copyAndResizeLayer(DataRepresentation* targetRep) const{
    //ivwAssert(false, "Not implemented");

    // Make sure that the OpenCL layer is deleted before resizing the texture
    // TODO: Implement copying in addition to the resizing

    LayerCLGL* target = dynamic_cast<LayerCLGL*>(targetRep);
    const LayerCLGL* source = this;
    target->resize(source->getDimension());

    return true;
}

void LayerCLGL::notifyBeforeTextureInitialization() {
    delete clImage_; clImage_ = 0; 
}

void LayerCLGL::notifyAfterTextureInitialization() {
    clImage_ = new cl::Image2DGL(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, texture_->getID());
}




} // namespace

namespace cl {

template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::LayerCLGL& value)
{
    return setArg(index, value.get());
}


} // namespace cl
