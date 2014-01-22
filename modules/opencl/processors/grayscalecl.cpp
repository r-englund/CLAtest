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

#include "grayscalecl.h"
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/image/imagecl.h>
#include <modules/opencl/volume/volumecl.h>
#include <modules/opencl/kernelmanager.h>

namespace inviwo {

ProcessorClassName(GrayscaleCL, "GrayscaleCL"); 
ProcessorCategory(GrayscaleCL, "Image Operation");
ProcessorCodeState(GrayscaleCL, CODE_STATE_EXPERIMENTAL);

GrayscaleCL::GrayscaleCL()
    : Processor(),
    inputPort_("color image"),
    outport_("outport"),
    kernel_(NULL)
{
    addPort(inputPort_, "ImagePortGroup1");
    addPort(outport_, "ImagePortGroup1");
}

GrayscaleCL::~GrayscaleCL() {}

void GrayscaleCL::initialize() {
    Processor::initialize();
    try {
        cl::Program* program = KernelManager::getRef().buildProgram(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+"opencl/cl/grayscale.cl");
        kernel_ = KernelManager::getRef().getKernel(program, "grayscaleKernel");
    } catch (cl::Error&) {
        
    }
}

void GrayscaleCL::deinitialize() {
    Processor::deinitialize();
}

void GrayscaleCL::process() {
    if( kernel_ == NULL) {
        return;
    }
    
    Image* outImage = outport_.getData();
    const Image* inImage = inputPort_.getData();

    const ImageCL* colorImageCL = inImage->getRepresentation<ImageCL>();
    outImage->resize(inImage->getDimension());
    uvec2 outportDim = outImage->getDimension();
    ImageCL* outImageCL = outImage->getEditableRepresentation<ImageCL>();

    cl_uint arg = 0;
    kernel_->setArg(arg++, *colorImageCL->getLayerCL());
    kernel_->setArg(arg++, *outImageCL->getLayerCL());

    OpenCL::instance()->getQueue().enqueueNDRangeKernel(*kernel_, cl::NullRange, static_cast<glm::svec2>(outportDim));

}

} // namespace
