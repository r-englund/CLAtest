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

#include <modules/opencl/buffer/bufferclgl.h>
#include <inviwo/core/util/assertion.h>

namespace inviwo {

BufferCLGL::BufferCLGL(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage, const BufferGL* data, cl_mem_flags readWriteFlag)
    : BufferRepresentation(size, format, type, usage), bufferGL_(data), readWriteFlag_(readWriteFlag), bufferGLObjectId_(NULL)
{
    if(data) {
        initialize(data);
    }
}

BufferCLGL::BufferCLGL( const BufferCLGL& rhs )
: BufferRepresentation(rhs.getSize(), rhs.getDataFormat(), rhs.getBufferType(), rhs.getBufferUsage()), bufferGL_(rhs.getBufferGL()), readWriteFlag_(rhs.readWriteFlag_)
{
    initialize(bufferGL_);
}

BufferCLGL::~BufferCLGL() { 
    deinitialize(); 
}

void BufferCLGL::initialize(const BufferGL* data) {
    ivwAssert(data != 0, "Cannot initialize with null OpenGL buffer");
    bufferGLObjectId_ = const_cast<BufferGL*>(data)->getBufferId();
    bufferGLObjectId_->increaseRefCount();
    buffer_ = new cl::BufferGL(OpenCL::instance()->getContext(), readWriteFlag_, data->getId());
    BufferCLGL::initialize();
}

BufferCLGL* BufferCLGL::clone() const {
    return new BufferCLGL(*this);
}

void BufferCLGL::deinitialize() {
    delete buffer_;
    buffer_ = 0; 
    if(bufferGLObjectId_->decreaseRefCount() <= 0) {
        delete bufferGLObjectId_; bufferGLObjectId_ = NULL;
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
