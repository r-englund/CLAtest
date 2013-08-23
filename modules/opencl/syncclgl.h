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
    SyncCLGL(): releaseEvent_(NULL), syncEvents_(NULL) {
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
    ~SyncCLGL() {
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