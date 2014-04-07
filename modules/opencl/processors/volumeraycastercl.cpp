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

#include "volumeraycastercl.h"
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/syncclgl.h>
#include <modules/opencl/image/imagecl.h>
#include <modules/opencl/image/imageclgl.h>
#include <modules/opencl/volume/volumecl.h>
#include <modules/opencl/volume/volumeclgl.h>
#include <modules/opencl/kernelmanager.h>

namespace inviwo {

ProcessorClassName(VolumeRaycasterCL, "VolumeRaycasterCL");
ProcessorCategory(VolumeRaycasterCL, "Volume Rendering");
ProcessorCodeState(VolumeRaycasterCL, CODE_STATE_EXPERIMENTAL);

VolumeRaycasterCL::VolumeRaycasterCL()
    : Processor(), ProcessorKernelOwner(this)
    , volumePort_("volume")
    , entryPort_("entry-points")
    , exitPort_("exit-points")
    , outport_("outport")
    , lightSourcePos_("lightSourcePos", "Light source position", vec3(1.0f), vec3(-1.0f), vec3(1.0f))
    , samplingRate_("samplingRate", "Sampling rate", 1.0f, 1.0f, 15.0f)
    , transferFunction_("transferFunction", "Transfer function", TransferFunction())
    , workGroupSize_("wgsize", "Work group size", ivec2(8, 8), ivec2(0), ivec2(256))
    , kernel_(NULL)
{
    addPort(volumePort_, "VolumePortGroup");
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addPort(outport_, "ImagePortGroup1");
    addProperty(samplingRate_);
    addProperty(lightSourcePos_);
    addProperty(transferFunction_);
    addProperty(workGroupSize_);
}

VolumeRaycasterCL::~VolumeRaycasterCL() {}

void VolumeRaycasterCL::initialize() {
    Processor::initialize();
    kernel_ = addKernel(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)
                 + "opencl/cl/volumeraycaster.cl", "raycaster");
}

void VolumeRaycasterCL::deinitialize() {
    Processor::deinitialize();
}

void VolumeRaycasterCL::process() {
    if (kernel_ == NULL) {
        return;
    }

    //const ImageCLGL* entryCLGL = entryPort_.getData()->getRepresentation<ImageCLGL>();
    const ImageCL* entryCLGL = entryPort_.getData()->getRepresentation<ImageCL>();
    // Will synchronize with OpenGL upon creation and destruction
    //SyncCLGL glSync;
    //entryCLGL->getLayerCL()->aquireGLObject(glSync.getGLSyncEvent());
    //const ImageCLGL* exitCLGL = exitPort_.getData()->getRepresentation<ImageCLGL>();
    const ImageCL* exitCLGL = exitPort_.getData()->getRepresentation<ImageCL>();
    //exitCLGL->getLayerCL()->aquireGLObject();
    Image* outImage = outport_.getData();
    ImageCL* outImageCL = outImage->getEditableRepresentation<ImageCL>();
    //ImageCLGL* outImageCL = outImage->getEditableRepresentation<ImageCLGL>();
    uvec2 outportDim = outImage->getDimension();
    //outImageCL->getLayerCL()->aquireGLObject();
    const Volume* volume = volumePort_.getData();
    //const VolumeCLGL* volumeCL = volume->getRepresentation<VolumeCLGL>();
    //volumeCL->aquireGLObject();
    const VolumeCL* volumeCL = volume->getRepresentation<VolumeCL>();
    uvec3 volumeDim = volumeCL->getDimension();
    const LayerCL* transferFunctionCL = transferFunction_.get().getData()->getRepresentation<LayerCL>();
    svec2 localWorkGroupSize(workGroupSize_.get());
    svec2 globalWorkGroupSize(getGlobalWorkGroupSize(outportDim.x, localWorkGroupSize.x), getGlobalWorkGroupSize(outportDim.y,
                              localWorkGroupSize.y));
#if IVW_PROFILING
    cl::Event* profilingEvent = new cl::Event();
#else
    cl::Event* profilingEvent = NULL;
#endif

    try
    {
        cl_uint arg = 0;
        kernel_->setArg(arg++, *volumeCL);
        kernel_->setArg(arg++, *entryCLGL->getLayerCL());
        kernel_->setArg(arg++, *exitCLGL->getLayerCL());
        kernel_->setArg(arg++, *transferFunctionCL);
        kernel_->setArg(arg++, samplingRate_.get());
        kernel_->setArg(arg++, volumeDim);
        kernel_->setArg(arg++, *outImageCL->getLayerCL());
        //
        OpenCL::getPtr()->getQueue().enqueueNDRangeKernel(*kernel_, cl::NullRange, globalWorkGroupSize, localWorkGroupSize, NULL, profilingEvent);
    } catch (cl::Error& err) {
        LogError(getCLErrorString(err));
    }

    //volumeCL->releaseGLObject();
    //outImageCL->getLayerCL()->releaseGLObject();
    //exitCLGL->getLayerCL()->releaseGLObject();
    //entryCLGL->getLayerCL()->releaseGLObject(NULL, glSync.getLastReleaseGLEvent());
#if IVW_PROFILING

    try {
        profilingEvent->wait();
        LogInfo("Exec time: " << profilingEvent->getElapsedTime() << " ms");
    } catch (cl::Error& err) {
        LogError(getCLErrorString(err));
    }

    delete profilingEvent;
#endif
}

} // namespace
