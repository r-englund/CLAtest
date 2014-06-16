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
#include <modules/opencl/kernelmanager.h>
#include <modules/opencl/image/imagecl.h>
#include <modules/opencl/image/imageclgl.h>
#include <modules/opencl/settings/openclsettings.h> // To check if the we can use sharing
#include <modules/opencl/syncclgl.h>
#include <modules/opencl/volume/volumecl.h>
#include <modules/opencl/volume/volumeclgl.h>


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
    , samplingRate_("samplingRate", "Sampling rate", 1.0f, 1.0f, 15.0f)
    , transferFunction_("transferFunction", "Transfer function", TransferFunction())
    , workGroupSize_("wgsize", "Work group size", ivec2(8, 8), ivec2(0), ivec2(256))
    , useGLSharing_("glsharing", "Use OpenGL sharing", true)
    , kernel_(NULL)
{
    addPort(volumePort_, "VolumePortGroup");
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addPort(outport_, "ImagePortGroup1");
    addProperty(samplingRate_);
    addProperty(transferFunction_);
    addProperty(workGroupSize_);
    addProperty(useGLSharing_);
}

VolumeRaycasterCL::~VolumeRaycasterCL() {}

void VolumeRaycasterCL::initialize() {
    Processor::initialize();
    // Will compile kernel and make sure that it it
    // recompiled whenever the file changes
    // If the kernel fails to compile it will be set to NULL
    kernel_ = addKernel("volumeraycaster.cl", "raycaster");

    if (!InviwoApplication::getPtr()->getSettingsByType<OpenCLSettings>()->isSharingEnabled()) {
        useGLSharing_.setReadOnly(true);
        useGLSharing_.set(false);
    }
}

void VolumeRaycasterCL::deinitialize() {
    Processor::deinitialize();
}

bool VolumeRaycasterCL::isReady() const {
    return Processor::isReady() && kernel_ != NULL;
}

void VolumeRaycasterCL::process() {

    Image* outImage = outport_.getData();
    uvec2 outportDim = outImage->getDimension();
    const Volume* volume = volumePort_.getData();
    //uvec3 volumeDim = volume->getDimension(); //Not used
    
    svec2 localWorkGroupSize(workGroupSize_.get());
    svec2 globalWorkGroupSize(getGlobalWorkGroupSize(outportDim.x, localWorkGroupSize.x), getGlobalWorkGroupSize(outportDim.y,
        localWorkGroupSize.y));

    try {
        // This macro will create an event called profilingEvent if IVW_PROFILING is enabled,
        // otherwise the profilingEvent will be declared as a null pointer
        IVW_OPENCL_PROFILING(profilingEvent, "")

        if (useGLSharing_.get()) {
            // SyncCLGL will synchronize with OpenGL upon creation and destruction
            SyncCLGL glSync;
            const ImageCLGL* entryCL = entryPort_.getData()->getRepresentation<ImageCLGL>();
            const ImageCLGL* exitCL = exitPort_.getData()->getRepresentation<ImageCLGL>();
            ImageCLGL* outImageCL = outImage->getEditableRepresentation<ImageCLGL>();
            const VolumeCLGL* volumeCL = volume->getRepresentation<VolumeCLGL>();
            const LayerCLGL* transferFunctionCL = transferFunction_.get().getData()->getRepresentation<LayerCLGL>();
            // Shared objects must be acquired before use.
            glSync.addToAquireGLObjectList(entryCL);
            glSync.addToAquireGLObjectList(exitCL);
            glSync.addToAquireGLObjectList(outImageCL);
            glSync.addToAquireGLObjectList(volumeCL);
            glSync.addToAquireGLObjectList(transferFunctionCL);
            // Acquire all of the objects at once
            glSync.aquireAllObjects();
            vec2 volumeDataOffsetScaling = volumeCL->getVolumeDataOffsetAndScaling(volume);
            volumeRaycast(volumeCL, volumeDataOffsetScaling, entryCL->getLayerCL(), exitCL->getLayerCL(), transferFunctionCL, outImageCL->getLayerCL(), globalWorkGroupSize, localWorkGroupSize, profilingEvent);
        } else {
            const ImageCL* entryCL = entryPort_.getData()->getRepresentation<ImageCL>();
            const ImageCL* exitCL = exitPort_.getData()->getRepresentation<ImageCL>();
            ImageCL* outImageCL = outImage->getEditableRepresentation<ImageCL>();
            const VolumeCL* volumeCL = volume->getRepresentation<VolumeCL>();
            const LayerCL* transferFunctionCL = transferFunction_.get().getData()->getRepresentation<LayerCL>();
            vec2 volumeDataOffsetScaling = volumeCL->getVolumeDataOffsetAndScaling(volume);
            volumeRaycast(volumeCL, volumeDataOffsetScaling, entryCL->getLayerCL(), exitCL->getLayerCL(), transferFunctionCL, outImageCL->getLayerCL(), globalWorkGroupSize, localWorkGroupSize, profilingEvent);
        }

        // This macro will destroy the profiling event and print the timing in the console if IVW_PROFILING is enabled
        
    } catch (cl::Error& err) {
        LogError(getCLErrorString(err));
    }
}

void VolumeRaycasterCL::volumeRaycast(const VolumeCLBase* volumeCL, const vec2& volumeDataOffsetScaling, const LayerCLBase* entryCLGL, const LayerCLBase* exitCLGL, const LayerCLBase* transferFunctionCL, LayerCLBase* outImageCL, svec2 globalWorkGroupSize, svec2 localWorkGroupSize, cl::Event* profilingEvent)
{
    // Note that the overloaded setArg methods requires 
    // the reference to an object (not the pointer), 
    // which is why we need to dereference the pointers
    cl_uint arg = 0;
    kernel_->setArg(arg++, *volumeCL);
    kernel_->setArg(arg++, volumeDataOffsetScaling); // Scaling for 12-bit data
    kernel_->setArg(arg++, *entryCLGL);
    kernel_->setArg(arg++, *exitCLGL);
    kernel_->setArg(arg++, *transferFunctionCL);
    kernel_->setArg(arg++, samplingRate_.get());
    kernel_->setArg(arg++, *outImageCL);
    //
    OpenCL::getPtr()->getQueue().enqueueNDRangeKernel(*kernel_, cl::NullRange, globalWorkGroupSize, localWorkGroupSize, NULL, profilingEvent);
}

} // namespace
