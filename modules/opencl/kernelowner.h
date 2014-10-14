/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Contact: Daniel Jönsson
 *
 *********************************************************************************/

#ifndef IVW_KERNEL_OWNER_H
#define IVW_KERNEL_OWNER_H

#include <modules/opencl/openclmoduledefine.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {
/** \class KernelOwner
 * Interface for processors that use OpenCL features.
 * Inherit from this class to enable invalidation of processor
 * when a kernel has been compiled.
 * @see ProcessorKernelOwner
 */
class IVW_MODULE_OPENCL_API KernelOwner {
public:
    KernelOwner() {};
    virtual ~KernelOwner();

    /**
     * Override this method to perform operations after a successful kernel 
     * compilation. 
     * @param kernel The kernel that was compiled
     */
    virtual void onKernelCompiled(const cl::Kernel* kernel) = 0;

    /**
     * Builds, adds and returns the kernel if successfully built. 
     * @note Do not delete the returned kernel
     * @param filePath Path to file containing kernel
     * @param kernelName Name of kernel
     * @param defines Defines to be set when building kernel
     * @return bool Kernel if successfully built, otherwise NULL
     */
    cl::Kernel* addKernel(const std::string& filePath, const std::string& kernelName, const std::string& defines = "");

    const std::set<cl::Kernel*>& getKernels() const { return kernels_; }

protected:
    std::set<cl::Kernel*> kernels_;
};
/** \class ProcessorKernelOwner
* Convenience class for processors. Will call invalidate with INVALID_RESOURCES
* when a kernel has been recompiled
*/
class IVW_MODULE_OPENCL_API ProcessorKernelOwner: public KernelOwner {
public:
    ProcessorKernelOwner(Processor* processor);
    virtual ~ProcessorKernelOwner() {};

    /**
     * Calls invalidate on Processor specified in the constructor.
     * Override this method to perform operations after a successful kernel 
     * compilation. 
     * @param kernel The kernel that was compiled
     */
    virtual void onKernelCompiled(const cl::Kernel* kernel);
protected:
    Processor* processor_;
};

} // namespace

#endif // IVW_KERNEL_OWNER_H
