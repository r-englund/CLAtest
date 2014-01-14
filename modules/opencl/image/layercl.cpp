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

#include <modules/opencl/image/layercl.h>
#include <modules/opencl/image/layerclresizer.h>
#include <inviwo/core/util/assertion.h>
namespace inviwo {

LayerCL::LayerCL(uvec2 dimensions, LayerType type, const DataFormatBase* format, const void* data)
    : LayerRepresentation(dimensions, type, format), layerFormat_(dataFormatToCLImageFormat(format->getId()))
{
    initialize(data);
}

LayerCL::LayerCL( const LayerCL& rhs )
    : LayerRepresentation(rhs.getDimension(), rhs.getLayerType(), rhs.getDataFormat()), layerFormat_(dataFormatToCLImageFormat(rhs.getDataFormat()->getId()))
{
    initialize(NULL);
    OpenCL::instance()->getQueue().enqueueCopyImage(rhs.get(), *clImage2D_ , glm::svec3(0), glm::svec3(0), glm::svec3(dimensions_, 1));
}

LayerCL::~LayerCL() { 
    deinitialize(); 
}

void LayerCL::initialize(const void* texels) {

    if (texels != NULL) {
        // Could performance be increased by using pinned memory?
        // 3.1.1 http://www.nvidia.com/content/cudazone/CUDABrowser/downloads/papers/NVIDIA_OpenCL_BestPracticesGuide.pdf
        //cl::Buffer pinnedMem(OpenCL::instance()->getContext(), CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR, sizeof(texels), NULL, NULL);
        //unsigned char* mappedMem = (unsigned char*)OpenCL::instance()->getQueue().enqueueMapBuffer(pinnedMem, true, CL_MAP_WRITE, 0, sizeof(texels), 0);
        //memcpy(mappedMem, texels, sizeof(texels));
        //OpenCL::instance()->getQueue().enqueueWriteLayer(*layer2D_, true, glm::svec3(0), glm::svec3(dimensions_, 1), 0, 0, mappedMem);
        //OpenCL::instance()->getQueue().enqueueUnmapMemObject(pinnedMem, mappedMem);

        // This should also use pinned memory...
        clImage2D_ = new cl::Image2D(OpenCL::instance()->getContext(), 
            CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR | CL_MEM_ALLOC_HOST_PTR, 
            getFormat(), static_cast<size_t>(dimensions_.x), static_cast<size_t>(dimensions_.y), 0, const_cast<void*>(texels));
        // Alternatively first allocate memory on device and then transfer
        //layer2D_ = new cl::Layer2D(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, getFormat(), dimensions_.x, dimensions_.y);
        //OpenCL::instance()->getQueue().enqueueWriteLayer(*layer2D_, true, glm::svec3(0), glm::svec3(dimensions_, 1), 0, 0, texels);
    } else {
        clImage2D_ = new cl::Image2D(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, getFormat(), static_cast<size_t>(dimensions_.x), static_cast<size_t>(dimensions_.y));
    }
    LayerCL::initialize();
}

DataRepresentation* LayerCL::clone() const {
    LayerCL* newLayerCL = new LayerCL(dimensions_, getLayerType(), getDataFormat());
    OpenCL::instance()->getQueue().enqueueCopyImage(*clImage2D_, (newLayerCL->get()), glm::svec3(0), glm::svec3(0), glm::svec3(dimensions_, 1));
    return newLayerCL;
}

void LayerCL::deinitialize() {
    delete clImage2D_;
    clImage2D_ = 0;
}

void LayerCL::upload( const void* data )
{
    OpenCL::instance()->getQueue().enqueueWriteImage(*clImage2D_, true, glm::svec3(0), glm::svec3(dimensions_, 1), 0, 0, const_cast<void*>(data));
}

void LayerCL::download( void* data ) const
{
    OpenCL::instance()->getQueue().enqueueReadImage(*clImage2D_, true, glm::svec3(0), glm::svec3(dimensions_, 1), 0, 0, data);
}

void LayerCL::resize(uvec2 dimensions)
{
    if (dimensions == dimensions_) {
        return;
    }
    cl::Image2D* resizedLayer2D = new cl::Image2D(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, getFormat(), dimensions.x, dimensions.y);
    LayerCLResizer::resize(*clImage2D_, *resizedLayer2D, dimensions);
    delete clImage2D_;
    clImage2D_ = resizedLayer2D;
    LayerRepresentation::resize(dimensions);

}

bool LayerCL::copyAndResizeLayer(DataRepresentation* target) const{
    LayerCL* targetCL = dynamic_cast<LayerCL*>(target);

    if (!targetCL) return false;

    LayerCLResizer::resize(*clImage2D_, (targetCL->get()), targetCL->getDimension());
	
	return true;
}

void LayerCL::setDimension( uvec2 dimensions )
{
    delete clImage2D_;
    clImage2D_ = new cl::Image2D(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, getFormat(), dimensions.x, dimensions.y);
}

} // namespace

namespace cl {

template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::LayerCL& value) {
    return setArg(index, value.get());
}

} // namespace cl
