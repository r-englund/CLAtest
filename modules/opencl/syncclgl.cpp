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

#include <modules/opencl/syncclgl.h>

/*
typedef cl_event (*PFNCLCREATEEVENTFROMGLSYNCKHR) (cl_context context, cl_GLsync sync, cl_int *errcode_ret);
PFNCLCREATEEVENTFROMGLSYNCKHR clCreateEventFromGLsync = (PFNCLCREATEEVENTFROMGLSYNCKHR)clGetExtensionFunctionAddress("clCreateEventFromGLsyncKHR");
*/

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
    try {
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
    } catch (cl::Error& err) {
        LogError(getCLErrorString(err));
    }
}

} // end namespace
