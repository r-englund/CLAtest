/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 *********************************************************************************/

#include <modules/pypackages/processors/pycuda/pycudaimageinvert.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/datastructures/image/imageramprecision.h>
#include <modules/pypackages/pypackagesmodule.h>

namespace inviwo {

ProcessorClassIdentifier(PyCUDAImageInverter, "org.inviwo.PyCUDAImageInverter");
ProcessorDisplayName(PyCUDAImageInverter,  "PyCUDA Image Inverter");
ProcessorTags(PyCUDAImageInverter, "Python/CUDA");
ProcessorCategory(PyCUDAImageInverter, "Numpy");
ProcessorCodeState(PyCUDAImageInverter, CODE_STATE_EXPERIMENTAL);

PyCUDAImageInverter::PyCUDAImageInverter()
    : PyProcessorBase()
    , inport_("inport")
    , outport_("outport")
    , cudaKernelFile_("cudaKernel", "Edit Kernel",
                      InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES) +
                          "pypackages/scripts/pycuda/pycudaimageinvert.cu",
                      "default", INVALID_OUTPUT, PropertySemantics("TextEditor"))
    , cachedImageData_(0) {
    addPort(inport_);
    addPort(outport_);
    addProperty(cudaKernelFile_);

    pythonScriptFile_.set(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES) +
                          "pypackages/scripts/pycuda/pycudaimageinvert.py");

    cudaKernelFile_.onChange(this, &PyCUDAImageInverter::loadCUDAKernelFile);
    inport_.onChange(this, &PyCUDAImageInverter::onInportChange);
}

PyCUDAImageInverter::~PyCUDAImageInverter() {}

void PyCUDAImageInverter::initialize() {
    PyProcessorBase::initialize();
    allocateBuffers();
}

void PyCUDAImageInverter::deinitialize() {
    PyProcessorBase::deinitialize();
    deAllocateBuffers();
}

void PyCUDAImageInverter::onInportChange() {
    if (!inport_.hasData()) { 
        cachedImageData_ = 0; 
        return; 
    }

    bool pycudaAvailable = PyScriptRunner::getPtr()->isPackageAvailable("pycuda");
    //Check if pycuda package is available
    if (!pycudaAvailable)
        LogWarn("PyCUDA pacakage not available")

    cachedImageData_ = inport_.getData()->clone();
}

void PyCUDAImageInverter::process() {
    if (!cachedImageData_) {
        LogInfo("Cached data is not allocated. Inport has no data.")
        outport_.setData(cachedImageData_, false);
        return;
    }
    loadCUDAKernelFile();
    PyProcessorBase::addExistingLayer("SourceImage", cachedImageData_->getColorLayer());
    PyProcessorBase::process();
    outport_.setData(cachedImageData_, false);
}

void PyCUDAImageInverter::allocateBuffers() {
    //load kernel file into string buffer
    loadCUDAKernelFile();
    //default image layer
    //TODO: memory leak while replacing this layer
    PyProcessorBase::allocateLayer("SourceImage", DataVec4UINT8::str(), ivec2(256) );
}

void PyCUDAImageInverter::deAllocateBuffers() {
    PyProcessorBase::freeAll();
}

void PyCUDAImageInverter::loadCUDAKernelFile() {
    std::string scriptFileName = cudaKernelFile_.get();
    std::ifstream file(scriptFileName.c_str());
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    size_t bufferSize = text.size();

    if (!bufferSize) {
        LogWarn("Invalid CUDA Kernel");
        return;
    }

    //allocate buffer for cuda kernel code (char buffer)
    std::string buffName = "CUDAKernelSrc";
    Buffer* buff =  PyProcessorBase::getAllocatedPyBuffer(buffName);
    if (buff) {
        if (buff->getSize()!=bufferSize) {
             PyProcessorBase::deallocatePyBuffer(buffName);
        }
    }
    PyProcessorBase::allocatePyBuffer(buffName, DataUINT8::str(), bufferSize);
    void* kernelSrc = 0;
    //fetch buffer data
    if (PyProcessorBase::isValidPyBuffer(buffName)) {
        kernelSrc = PyProcessorBase::getPyBufferData(buffName);
        if (kernelSrc) {
            char* csrc = static_cast<char*>(kernelSrc);
            //copy to buffer data which is later accessed by python
            std::strncpy(csrc, text.c_str(), bufferSize);
        }
        else
            LogWarn("Invalid buffer data requested")
    }
}

} // namespace
