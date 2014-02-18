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
        cl::Program* program = KernelManager::getRef().buildProgram(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)
                               +"opencl/cl/grayscale.cl");
        kernel_ = KernelManager::getRef().getKernel(program, "grayscaleKernel");
    } catch (cl::Error&) {
    }
}

void GrayscaleCL::deinitialize() {
    Processor::deinitialize();
}

void GrayscaleCL::process() {
    if (kernel_ == NULL) {
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
