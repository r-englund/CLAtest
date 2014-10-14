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
 * Contact: Erik Sundén
 *
 *********************************************************************************/

#include <modules/opencl/kernelowner.h>
#include <modules/opencl/kernelmanager.h>
#include <inviwo/core/util/urlparser.h>

namespace inviwo {

cl::Kernel* KernelOwner::addKernel(const std::string& filePath,
                            const std::string& kernelName,
                            const std::string& defines /*= ""*/ ) {
    if (filePath.length() > 0) {
        std::string absoluteFileName = filePath;
        if (!URLParser::fileExists(absoluteFileName)) {
            // Search in include directories added by modules
            const std::vector<std::string> openclSearchPaths = OpenCL::getPtr()->getCommonIncludeDirectories();

            for (size_t i=0; i<openclSearchPaths.size(); i++) {
                if (URLParser::fileExists(openclSearchPaths[i]+"/"+filePath)) {
                    absoluteFileName = openclSearchPaths[i]+"/"+filePath;
                    break;
                }
            }
        }

        cl::Program* program = KernelManager::getPtr()->buildProgram(absoluteFileName, defines);

        cl::Kernel* kernel = KernelManager::getPtr()->getKernel(program, kernelName, this);
        if (kernel) {
            kernels_.insert(kernel);
        }
        return kernel;

    } else return NULL;
}

KernelOwner::~KernelOwner() {
    // Make sure that we are not notifed after destruction
    KernelManager::getPtr()->stopObservingKernels(this);
}

ProcessorKernelOwner::ProcessorKernelOwner(Processor* processor)
    : KernelOwner()
    , processor_(processor) {
}

void ProcessorKernelOwner::onKernelCompiled( const cl::Kernel* kernel ) {
    processor_->invalidate(PropertyOwner::INVALID_RESOURCES);
}

} // namespace
