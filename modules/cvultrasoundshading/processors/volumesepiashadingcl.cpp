/**********************************************************************
 * Copyright (C) 2014 ContextVision AB
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

#include <modules/cvultrasoundshading/processors/volumesepiashadingcl.h>
#include <inviwo/core/datastructures/light/directionallight.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/syncclgl.h>
#include <modules/opencl/image/imagecl.h>
#include <modules/opencl/image/imageclgl.h>
#include <modules/opencl/volume/volumecl.h>
#include <modules/opencl/volume/volumeclgl.h>
#include <modules/opencl/kernelmanager.h>

namespace inviwo {

ProcessorClassName(VolumeSepiaShadingCL, "VolumeSepiaShadingCL"); 
ProcessorCategory(VolumeSepiaShadingCL, "Volume Rendering");
ProcessorCodeState(VolumeSepiaShadingCL, CODE_STATE_EXPERIMENTAL);

VolumeSepiaShadingCL::VolumeSepiaShadingCL()
    : Processor()
    , volumePort_("volume")
    , entryPort_("entry-points")
    , exitPort_("exit-points")
    , lightSource_("lightSource")
    , outport_("outport", COLOR_ONLY, DataVec4FLOAT32::get())
    , advancedMaterial_("material", "Material")
    , samplingRate_("samplingRate", "Sampling rate", 1.0f, 1.0f, 15.0f)
    , transferFunction_("transferFunction", "Transfer function", TransferFunction())
    , workGroupSize_("wgsize", "Work group size", ivec2(8, 8), ivec2(0), ivec2(256))
    , useGLSharing_("glsharing", "Use OpenGL sharing", true)
    , kernel_(NULL)
{
    addPort(volumePort_, "VolumePortGroup");
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");
    addPort(lightSource_, "LightPortGroup");
    addPort(outport_, "ImagePortGroup1");

    addProperty(advancedMaterial_);
    addProperty(samplingRate_);
    addProperty(transferFunction_);
    addProperty(workGroupSize_);
    addProperty(useGLSharing_);
}

VolumeSepiaShadingCL::~VolumeSepiaShadingCL() {}

void VolumeSepiaShadingCL::initialize() {
    Processor::initialize();
    try {
        cl::Program* program = KernelManager::getRef().buildProgram(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+"cvultrasoundshading/cl/volumesepiashading.cl");
        kernel_ = KernelManager::getRef().getKernel(program, "volumeSepiaShading");

    } catch (cl::Error&) {
        
    }

}

void VolumeSepiaShadingCL::deinitialize() {
    Processor::deinitialize();
}

void VolumeSepiaShadingCL::process() {
    const DirectionalLight* light = dynamic_cast<const DirectionalLight*>(lightSource_.getData());
    if( kernel_ == NULL || light == NULL) {
        return;
    }

#if IVW_PROFILING
    cl::Event* profilingEvent = new cl::Event(); 
#else 
    cl::Event* profilingEvent = NULL;
#endif
    if (useGLSharing_.get()) {
        // Will synchronize with OpenGL upon creation and destruction
        SyncCLGL glSync;
        const ImageCLGL* entry = entryPort_.getData()->getRepresentation<ImageCLGL>();
        const ImageCLGL* exit = exitPort_.getData()->getRepresentation<ImageCLGL>();
        const ImageCLGL* output = outport_.getData()->getEditableRepresentation<ImageCLGL>();
        const VolumeCLGL* volume = volumePort_.getData()->getRepresentation<VolumeCLGL>();
        const LayerCLGL* tfCL = transferFunction_.get().getData()->getRepresentation<LayerCLGL>();
        volume->aquireGLObject(glSync.getGLSyncEvent());
        entry->getLayerCL()->aquireGLObject();
        exit->getLayerCL()->aquireGLObject();
        output->getLayerCL()->aquireGLObject();
        tfCL->aquireGLObject();

        compute(*volume, *entry->getLayerCL(), *exit->getLayerCL(), *tfCL, *output->getLayerCL(), light, profilingEvent);
        entry->getLayerCL()->releaseGLObject();
        exit->getLayerCL()->releaseGLObject();
        output->getLayerCL()->releaseGLObject();
        tfCL->releaseGLObject();
        volume->releaseGLObject(NULL, glSync.getLastReleaseGLEvent());
    } else {
        const ImageCL* entry = entryPort_.getData()->getRepresentation<ImageCL>();
        const ImageCL* exit = exitPort_.getData()->getRepresentation<ImageCL>();
        const ImageCL* output = outport_.getData()->getEditableRepresentation<ImageCL>();
        const VolumeCL* volume = volumePort_.getData()->getRepresentation<VolumeCL>();
        const LayerCL* tfCL = transferFunction_.get().getData()->getRepresentation<LayerCL>();
        const cl::Image& volumeCL = volume->get();
        const cl::Image& entryCL = entry->getLayerCL()->get();
        const cl::Image& exitCL = exit->getLayerCL()->get();
        const cl::Image& outImageCL = output->getLayerCL()->get();
        const cl::Image& tf = tfCL->get();
        compute(*volume, *entry->getLayerCL(), *exit->getLayerCL(), *tfCL, *output->getLayerCL(), light, profilingEvent);
    }
    
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

void VolumeSepiaShadingCL::compute( const VolumeCLBase& volumeCL, const LayerCLBase& entryPoints, const LayerCLBase& exitPoints, const LayerCLBase& transferFunctionCL, const LayerCLBase& output, const DirectionalLight* light, cl::Event* profilingEvent )
{
    const mat4& textureToIndexMatrix = volumePort_.getData()->getCoordinateTransformer().getTextureToIndexMatrix();
    vec3 voxelSpacing(1.f/glm::length(textureToIndexMatrix[0]), 1.f/glm::length(textureToIndexMatrix[1]), 1.f/glm::length(textureToIndexMatrix[2]));
    float stepSize = samplingRate_.get()*static_cast<float>(std::min(voxelSpacing.x, std::min(voxelSpacing.y, voxelSpacing.z)));
    const vec3& lightDirection = light->getDirection();
    const vec3& lightPower = light->getIntensity();
    ShadingFunctionEnum::Enum scatteringFunction = advancedMaterial_.getPhaseFunctionEnum();

    svec2 localWorkGroupSize(workGroupSize_.get());
    svec2 globalWorkGroupSize(getGlobalWorkGroupSize(entryPort_.getData()->getDimension().x, localWorkGroupSize.x), getGlobalWorkGroupSize(entryPort_.getData()->getDimension().y, localWorkGroupSize.y));

    
    try {
        cl_uint arg = 0;
        kernel_->setArg(arg++, volumeCL);
        kernel_->setArg(arg++, entryPoints);
        kernel_->setArg(arg++, exitPoints);
        kernel_->setArg(arg++, transferFunctionCL);
        kernel_->setArg(arg++, stepSize);
        kernel_->setArg(arg++, voxelSpacing);
        kernel_->setArg(arg++, lightDirection);
        kernel_->setArg(arg++, lightPower);
        kernel_->setArg(arg++, scatteringFunction);
        kernel_->setArg(arg++, advancedMaterial_.specularColorProp.get());
        kernel_->setArg(arg++, advancedMaterial_.getCombinedMaterialParameters());
        kernel_->setArg(arg++, output);
        OpenCL::instance()->getQueue().enqueueNDRangeKernel(*kernel_, cl::NullRange, globalWorkGroupSize, localWorkGroupSize, NULL, profilingEvent);
    } catch (cl::Error& err) {
        LogError(getCLErrorString(err));
    }
}

} // namespace


