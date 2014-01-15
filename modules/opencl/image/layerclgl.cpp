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
    clImage2DGL_ = new cl::Image2DGL(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, texture->getID());
    texture->addObserver(this);
    LayerCLGL::initialize();
}

LayerCLGL* LayerCLGL::clone() const {
    Texture2D* clonedTexture = new Texture2D(*texture_); 
    LayerCLGL* newLayerCLGL = new LayerCLGL(dimensions_, getLayerType(), getDataFormat(), clonedTexture);
    return newLayerCLGL;
}

void LayerCLGL::deinitialize() {
    delete clImage2DGL_;
    clImage2DGL_ = 0; 
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
    delete clImage2DGL_; clImage2DGL_ = 0; 
}

void LayerCLGL::notifyAfterTextureInitialization() {
    clImage2DGL_ = new cl::Image2DGL(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, texture_->getID());
}




} // namespace

namespace cl {

template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::LayerCLGL& value)
{
    return setArg(index, value.get());
}


} // namespace cl
