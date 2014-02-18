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

#include <modules/opencl/buffer/bufferclgl.h>
#include <modules/opencl/openclsharing.h>
#include <inviwo/core/util/assertion.h>

namespace inviwo {

BufferCLGL::BufferCLGL(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage, BufferObject* data,
                       cl_mem_flags readWriteFlag)
    : BufferRepresentation(size, format, type, usage), bufferGL_(data), readWriteFlag_(readWriteFlag)
{
    if (data) {
        initialize(data);
    }
}

BufferCLGL::BufferCLGL(const BufferCLGL& rhs)
    : BufferRepresentation(rhs), bufferGL_(rhs.getBufferGL()), readWriteFlag_(rhs.readWriteFlag_)
{
    initialize(bufferGL_);
}

BufferCLGL::~BufferCLGL() {
    deinitialize();
}

void BufferCLGL::initialize(BufferObject* data) {
    ivwAssert(data != 0, "Cannot initialize with null OpenGL buffer");
    data->increaseRefCount();
    CLBufferSharingMap::iterator it = OpenCLBufferSharing::clBufferSharingMap_.find(data);

    if (it == OpenCLBufferSharing::clBufferSharingMap_.end()) {
        buffer_ = new cl::BufferGL(OpenCL::instance()->getContext(), readWriteFlag_, data->getId());
        OpenCLBufferSharing::clBufferSharingMap_.insert(BufferSharingPair(data, new OpenCLBufferSharing(buffer_)));
    } else {
        buffer_ = static_cast<cl::BufferGL*>(it->second->sharedMemory_);
        it->second->increaseRefCount();
    }

    data->addObserver(this);
    BufferCLGL::initialize();
}

BufferCLGL* BufferCLGL::clone() const {
    return new BufferCLGL(*this);
}

void BufferCLGL::deinitialize() {
    CLBufferSharingMap::iterator it = OpenCLBufferSharing::clBufferSharingMap_.find(bufferGL_);

    if (it != OpenCLBufferSharing::clBufferSharingMap_.end()) {
        if (it->second->decreaseRefCount() == 0) {
            delete it->second->sharedMemory_;
            it->second->sharedMemory_ = 0;
            delete it->second;
            OpenCLBufferSharing::clBufferSharingMap_.erase(it);
        }
    }

    if (bufferGL_ && bufferGL_->decreaseRefCount() <= 0) {
        delete bufferGL_;
        bufferGL_ = NULL;
    }
}

void BufferCLGL::notifyBeforeBufferInitialization() {
    CLBufferSharingMap::iterator it = OpenCLBufferSharing::clBufferSharingMap_.find(bufferGL_);

    if (it != OpenCLBufferSharing::clBufferSharingMap_.end()) {
        if (it->second->decreaseRefCount() == 0) {
            delete it->second->sharedMemory_;
            it->second->sharedMemory_ = 0;
        }
    }

    bufferGL_ = 0;
}

void BufferCLGL::notifyAfterBufferInitialization() {
    CLBufferSharingMap::iterator it = OpenCLBufferSharing::clBufferSharingMap_.find(bufferGL_);

    if (it != OpenCLBufferSharing::clBufferSharingMap_.end()) {
        if (it->second->getRefCount() == 0) {
            it->second->sharedMemory_ = new cl::BufferGL(OpenCL::instance()->getContext(), readWriteFlag_, bufferGL_->getId());
        }

        buffer_ = static_cast<cl::BufferGL*>(it->second->sharedMemory_);
        it->second->increaseRefCount();
    }
}

} // namespace

namespace cl {

template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::BufferCLGL& value)
{
    return setArg(index, value.getBuffer());
}


} // namespace cl
