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

#ifndef IVW_LAYERCLGL_H
#define IVW_LAYERCLGL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/layerrepresentation.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>
#include <modules/opengl/glwrap/texture2d.h>

namespace inviwo {
/** \class LayerCLGL 
*
* LayerCLGL handles shared texture2D between OpenCL and OpenGL.
* It will make sure that the texture 
* is not released while a shared representation exist
* and also release and reattach the shared representation
* when the texture is resized (handled through the TexturObserver)
*
* @see Observable
*/
class IVW_MODULE_OPENCL_API LayerCLGL : public LayerRepresentation, public TextureObserver {

public:
    LayerCLGL(uvec2 dimensions = uvec2(64), LayerType type = COLOR_LAYER, const DataFormatBase* format = DataFormatBase::get(), Texture2D* data = NULL);
    virtual ~LayerCLGL();
    virtual std::string getClassName() const { return "LayerCLGL"; }
    virtual void initialize(){};
    virtual void deinitialize();
    virtual LayerCLGL* clone() const;
    
    void initialize(Texture2D* texture);

    virtual void setDimension(uvec2 dimensions) { dimensions_ = dimensions; deinitialize(); initialize(texture_); }
    virtual void resize(uvec2 dimensions);
    virtual bool copyAndResizeLayer(DataRepresentation* target) const;

    const cl::Image2DGL& get() const { return *(clImage2DGL_); }
    const Texture2D* getTexture() const { return texture_; }

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

    void aquireGLObject(std::vector<cl::Event>* syncEvents = NULL) const {
        std::vector<cl::Memory> syncLayers(1, *clImage2DGL_); 
        OpenCL::instance()->getQueue().enqueueAcquireGLObjects(&syncLayers, syncEvents);
    }
    void releaseGLObject(std::vector<cl::Event>* syncEvents = NULL, cl::Event* event= NULL) const {
        std::vector<cl::Memory> syncLayers(1, *clImage2DGL_); 
        OpenCL::instance()->getQueue().enqueueReleaseGLObjects(&syncLayers, syncEvents, event);
    }

protected:
    cl::Image2DGL* clImage2DGL_;
    Texture2D* texture_;
};

} // namespace

namespace cl {

// Kernel argument specializations for LayerCLGL type 
// (enables calling cl::Queue::setArg with LayerCLGL)
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const inviwo::LayerCLGL& value);

} // namespace cl



#endif // IVW_LAYERCLGL_H
