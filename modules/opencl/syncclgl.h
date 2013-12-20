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

#ifndef IVW_SYNC_CL_GL_H
#define IVW_SYNC_CL_GL_H

#include <modules/opencl/openclmoduledefine.h>
#include <modules/opencl/inviwoopencl.h>

#ifdef CL_VERSION_1_1
#ifdef __APPLE__
#include <OpenCL/cl_gl_ext.h>
#else
#include <CL/cl_gl_ext.h>
#endif
#endif




namespace inviwo {


class IVW_MODULE_OPENCL_API SyncCLGL {
public:
    SyncCLGL();
    ~SyncCLGL();

    std::vector<cl::Event>* getGLSyncEvent() { return syncEvents_; }

    cl::Event* getLastReleaseGLEvent() { return releaseEvent_; }

protected:
#if defined(CL_VERSION_1_1)
    GLsync glFenceSync_;
    cl::Event glSync_;
#endif
    cl::Event* releaseEvent_;
    std::vector<cl::Event>* syncEvents_;
};

}

#endif // IVW_SYNC_CL_GL_H