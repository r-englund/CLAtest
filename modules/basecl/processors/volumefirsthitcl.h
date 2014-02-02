/**********************************************************************
 * Copyright (C) 2014 Scientific Visualization Group - Linköping University
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

#ifndef IVW_VOLUME_FIRST_HIT_CL_H
#define IVW_VOLUME_FIRST_HIT_CL_H

#include <modules/basecl/baseclmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/transferfunctionproperty.h>

#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

class IVW_MODULE_BASECL_API VolumeFirstHitCL : public Processor {
public:
    VolumeFirstHitCL();
    ~VolumeFirstHitCL();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

    void firstHit( const cl::Image3D& volumeCL, const cl::Image2D& entryPoints, const cl::Image2D& exitPoints, const cl::Image2D& transferFunctionCL, const cl::Image2D& output, float stepSize, uvec2 outportDim, svec2 localWorkGroupSize, cl::Event* profilingEvent );

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

#endif // IVW_VOLUME_FIRST_HIT_CL_H
