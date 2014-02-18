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
 * Main file authors: Daniel Jönsson, Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/core/datastructures/buffer/buffer.h>
#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>
#include <inviwo/core/datastructures/buffer/bufferram.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>

namespace inviwo {


Buffer::Buffer(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage):
    Data(), size_(size), format_(format), type_(type), usage_(usage) {
}
Buffer::Buffer(const Buffer& rhs)
    : Data(rhs)
    , size_(rhs.size_)
    , format_(rhs.format_)
    , type_(rhs.type_)
    , usage_(rhs.usage_) {
}

Buffer& Buffer::operator=(const Buffer& that) {
    if (this != &that) {
        Data::operator=(that);
        size_ = that.size_;
        format_ = that.format_;
        type_ = that.type_;
        usage_ = that.usage_;
    }

    return *this;
}

Buffer* Buffer::clone() const {
    return new Buffer(*this);
}

Buffer::~Buffer() {
}

void Buffer::resize(size_t size) {
    if (size != size_) {
        size_ = size;

        for (size_t i=0; i<representations_.size(); i++) {
            // Static cast is ok because the representations must be BufferRepresentations.
            BufferRepresentation* bufferRepresentation = static_cast<BufferRepresentation*>(representations_[i]) ;
            bufferRepresentation->resize(size);
        }

        setAllRepresentationsAsInvalid();
    }
}

size_t Buffer::getSizeInBytes() {
    return size_ * format_->getBytesStored();
}

void Buffer::setSize(size_t size) {
    resize(size);
}



DataRepresentation* Buffer::createDefaultRepresentation() {
    return createBufferRAM(getSize(), format_, type_, usage_);
}

size_t Buffer::getSize() const
{
    // We need to update the size if a representation has changed size
    if (lastValidRepresentation_)
        const_cast<Buffer*>(this)->size_ = static_cast<const BufferRepresentation*>(lastValidRepresentation_)->getSize();

    return size_;
}

}

