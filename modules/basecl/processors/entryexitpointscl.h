/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
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

#ifndef IVW_ENTRYEXITPOINTS_CL_H
#define IVW_ENTRYEXITPOINTS_CL_H

#include <modules/basecl/baseclmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

class IVW_MODULE_BASECL_API EntryExitPointsCL : public Processor {
public:
    EntryExitPointsCL();
    virtual ~EntryExitPointsCL();
    
    InviwoProcessorInfo();

    virtual void initialize();
    virtual void deinitialize();

protected:
    virtual void process();

    void computeEntryExitPoints(const mat4& NDCToTextureMat, const vec4& camPosInTextureSpace, const cl::Image2D& entryPointsCL, const cl::Image2D& exitPointsCL, const uvec2& outportDim, cl::Event* profilingEvent);

private:
    GeometryInport geometryPort_;
    ImageOutport entryPort_;
    ImageOutport exitPort_;

    CameraProperty camera_;
    IntVec2Property workGroupSize_;
    BoolProperty useGLSharing_;

	cl::Kernel* entryExitKernel_;

};

} // namespace

#endif // IVW_ENTRYEXITPOINTS_CL_H
