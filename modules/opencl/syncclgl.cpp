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

#include <modules/opencl/syncclgl.h>

typedef cl_event (*PFNCLCREATEEVENTFROMGLSYNCKHR) (cl_context context, cl_GLsync sync, cl_int *errcode_ret);
PFNCLCREATEEVENTFROMGLSYNCKHR clCreateEventFromGLsync = (PFNCLCREATEEVENTFROMGLSYNCKHR)clGetExtensionFunctionAddress("clCreateEventFromGLsyncKHR");

namespace inviwo {

SyncCLGL::SyncCLGL(): releaseEvent_(NULL), syncEvents_(NULL) {
#if defined(CL_VERSION_1_1) && defined(GL_ARB_cl_event) && defined(CL_COMMAND_GL_FENCE_SYNC_OBJECT_KHR)
    // FIXME: Get unresolved symbol from clCreateEventFromGLsyncKHR, why? This has been reported as a bug to NVIDIA.
    //glFenceSync_ = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    //cl_int err;
    //glSync_ = clCreateEventFromGLsyncKHR(OpenCL::instance()->getContext()(), glFenceSync_, &err);
    //syncEvents_ = new std::vector<cl::Event>(1, glSync_);
    //if(err != CL_SUCCESS) {
    //    LogError("Failed to create sync event");
    //}
    //releaseEvent_ = new cl::Event();
    glFinish();
#else
    glFinish();
#endif
}

SyncCLGL::~SyncCLGL() {
#if defined(CL_VERSION_1_1) && defined(GL_ARB_cl_event) && defined(CL_COMMAND_GL_FENCE_SYNC_OBJECT_KHR)
    //GLsync clSync = glCreateSyncFromCLeventARB(OpenCL::instance()->getContext()(), (*releaseEvent_)(), 0);
    //glWaitSync(clSync, 0, GL_TIMEOUT_IGNORED);
    //glDeleteSync(glFenceSync_);
    //delete syncEvents_;
    //delete releaseEvent_;
    OpenCL::instance()->getQueue().finish();
#else 
    OpenCL::instance()->getQueue().finish();
#endif

}

} // end namespace
