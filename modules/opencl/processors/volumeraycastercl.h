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

#ifndef IVW_VOLUME_RAYCASTER_CL_H
#define IVW_VOLUME_RAYCASTER_CL_H

#include <modules/opencl/openclmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/transferfunctionproperty.h>

#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/image/layerclbase.h>
#include <modules/opencl/kernelowner.h>
#include <modules/opencl/volume/volumeclbase.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API VolumeRaycasterCL : public Processor, public ProcessorKernelOwner {
public:
    VolumeRaycasterCL();
    ~VolumeRaycasterCL();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

    virtual bool isReady() const;
protected:
    virtual void process();

    void volumeRaycast(const VolumeCLBase* volumeCL, const vec2& volumeDataOffsetScaling, const LayerCLBase* entryCLGL, const LayerCLBase* exitCLGL, const LayerCLBase* transferFunctionCL, LayerCLBase* outImageCL, svec2 globalWorkGroupSize, svec2 localWorkGroupSize, cl::Event* profilingEvent);

private:
    VolumeInport volumePort_;
    ImageInport entryPort_;
    ImageInport exitPort_;
    ImageOutport outport_;

    FloatProperty samplingRate_;
    TransferFunctionProperty transferFunction_;
    IntVec2Property workGroupSize_;

    BoolProperty useGLSharing_;

    cl::Kernel* kernel_;
};

} // namespace

#endif // IVW_VOLUME_RAYCASTER_CL_H
