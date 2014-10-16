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

#ifndef IVW_VOLUME_MAX_OPERATION_CL_H
#define IVW_VOLUME_MAX_OPERATION_CL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/buffer/buffer.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/processors/processor.h>

#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/kernelowner.h>
#include <modules/opencl/buffer/bufferclbase.h>
#include <modules/opencl/volume/volumeclbase.h>

#include <modules/basecl/baseclmoduledefine.h>


namespace inviwo {

class IVW_MODULE_BASECL_API VolumeMaxCL : public Processor, public ProcessorKernelOwner {

public:
    VolumeMaxCL();
    ~VolumeMaxCL();

    InviwoProcessorInfo();

    void initialize();

    void buildKernel();

    void deinitialize();


protected:
    virtual void process();
    void executeVolumeOperation(const VolumeCLBase* volumeCL, VolumeCLBase* volumeOutCL, const uvec3& outDim, const svec3& globalWorkGroupSize,
                                const svec3& localWorkgroupSize);
private:
    VolumeInport inport_;
    VolumeOutport outport_;

    IntProperty volumeRegionSize_;
    IntVec3Property workGroupSize_;
    BoolProperty useGLSharing_;

    bool supportsVolumeWrite_; // Does the OpenCL device support volume writes?
    Buffer* tmpVolume_; // Used if writing to a volume is not supported
    cl::Kernel* kernel_;

};

}

#endif // IVW_VOLUME_MAX_OPERATION_CL_H


