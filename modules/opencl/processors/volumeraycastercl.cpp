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
    : Processor()
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
    try {
        cl::Program* program = KernelManager::getRef().buildProgram(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+"opencl/cl/volumeraycaster.cl");
        kernel_ = KernelManager::getRef().getKernel(program, "raycaster");
        //cl::Program program = OpenCL::buildProgram(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+"opencl/cl/volumeraycaster.cl");
        //kernel_ = new cl::Kernel(program, "raycaster");

    } catch (cl::Error&) {
        
    }

}

void VolumeRaycasterCL::deinitialize() {
    Processor::deinitialize();
}

void VolumeRaycasterCL::process() {
    if( kernel_ == NULL) {
        return;
    }
    
    const ImageCLGL* entryCLGL = entryPort_.getData()->getRepresentation<ImageCLGL>();
    // Will synchronize with OpenGL upon creation and destruction
    SyncCLGL glSync;
    entryCLGL->getLayerCLGL()->aquireGLObject(glSync.getGLSyncEvent());

    const ImageCLGL* exitCLGL = exitPort_.getData()->getRepresentation<ImageCLGL>();
    exitCLGL->getLayerCLGL()->aquireGLObject();

    Image* outImage = outport_.getData();
    //ImageCL* outImageCL = outImage->getEditableRepresentation<ImageCL>();
    ImageCLGL* outImageCL = outImage->getEditableRepresentation<ImageCLGL>();
    uvec2 outportDim = outImage->getDimension();
    outImageCL->getLayerCLGL()->aquireGLObject();


    const Volume* volume = volumePort_.getData();
    //const VolumeCLGL* volumeCL = volume->getRepresentation<VolumeCLGL>();
    //volumeCL->aquireGLObject();
    const VolumeCL* volumeCL = volume->getRepresentation<VolumeCL>();
    uvec3 volumeDim = volumeCL->getDimension();

    const LayerCL* transferFunctionCL = transferFunction_.get().getData()->getRepresentation<LayerCL>();
    svec2 localWorkGroupSize(workGroupSize_.get());
    svec2 globalWorkGroupSize(getGlobalWorkGroupSize(outportDim.x, localWorkGroupSize.x), getGlobalWorkGroupSize(outportDim.y, localWorkGroupSize.y));
#if IVW_PROFILING
    cl::Event* profilingEvent = new cl::Event(); 
#else 
    cl::Event* profilingEvent = NULL;
#endif
    try
    {
        cl_uint arg = 0;
        kernel_->setArg(arg++, *volumeCL);
        kernel_->setArg(arg++, *entryCLGL->getLayerCLGL());
        kernel_->setArg(arg++, *exitCLGL->getLayerCLGL());
        kernel_->setArg(arg++, *transferFunctionCL);
        kernel_->setArg(arg++, samplingRate_.get());
        kernel_->setArg(arg++, volumeDim);
        kernel_->setArg(arg++, *outImageCL->getLayerCLGL());
        // 
        OpenCL::instance()->getQueue().enqueueNDRangeKernel(*kernel_, cl::NullRange, globalWorkGroupSize, localWorkGroupSize, NULL, profilingEvent);
    } catch (cl::Error& err) {
        LogError(getCLErrorString(err));
    }

    
    //volumeCL->releaseGLObject();
    outImageCL->getLayerCLGL()->releaseGLObject();
    exitCLGL->getLayerCLGL()->releaseGLObject();
    entryCLGL->getLayerCLGL()->releaseGLObject(NULL, glSync.getLastReleaseGLEvent());
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
