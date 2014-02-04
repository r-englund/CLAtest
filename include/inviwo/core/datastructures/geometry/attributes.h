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
 * Main file authors: Erik Sundén, Daniel Jönsson
 *
 *********************************************************************************/

#ifndef IVW_ATTRIBUTES_H
#define IVW_ATTRIBUTES_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/buffer/buffer.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>
#include <vector>

namespace inviwo {



//DataFormatBase* createDataFormatFromAttributeType(BufferType attributeType);

//
//class IVW_CORE_API AttributesBase {
//public:
//    AttributesBase() { buffer_ = new Buffer(0, POSITION_ATTRIB, createDataFormatFromAttributeType(POSITION_ATTRIB)); }
//    AttributesBase(BufferType attributeType, const DataFormatBase* format) { buffer_ = new Buffer(0, attributeType, format); }
//    virtual ~AttributesBase() {delete buffer_;}
//
//    Buffer* getBuffer() { return buffer_;}
//    const Buffer* getBuffer() const { return buffer_;}
//
//    virtual AttributesBase* clone() const { return new AttributesBase(*this); }
//
//    virtual BufferType getAttributeType() const { return buffer_->getBufferType(); }
//    virtual const DataFormatBase* getDataFormat() const { return buffer_->getDataFormat(); }
//    virtual unsigned int getElementSize() const { return buffer_->getDataFormat()->getBytesStored(); }
//    virtual unsigned int getNumberOfAttributes() const { return buffer_->getSize(); }
//    virtual unsigned int getDataSize() const { return buffer_->getSizeInBytes(); }
//protected:
//    Buffer* buffer_;
//
//};
//
//template<typename T, size_t B, BufferType A>
//class Attributes : public AttributesBase {
//
//public:
//    Attributes() : AttributesBase(A, DataFormat<T,B>::get()) {}
//    virtual ~Attributes(){ }
//
//    virtual AttributesBase* clone() const { return new Attributes(*this); }
//
//    std::vector<T> getAttributeContainer() const { return buffer_->getRepresentation<T>()->getDataContainer(); }
//
//private:
//
//};



//class IVW_CORE_API AttributesBase {
//public:
//    AttributesBase(){}
//    virtual ~AttributesBase(){}
//
//    virtual const void* getAttributes() const = 0;
//    virtual BufferType getAttributeType() const = 0;
//    virtual const DataFormatBase* getDataFormat() const = 0;
//    virtual unsigned int getElementSize() const = 0;
//    virtual unsigned int getNumberOfAttributes() const = 0;
//    virtual unsigned int getDataSize() const = 0;
//};

//template<typename T, size_t B, BufferType A>
//class IVW_CORE_API Attributes : public AttributesBase {
//
//public:
//    Attributes() : AttributesBase(), dataFormat_(DataFormat<T,B>::get()) {}
//    virtual ~Attributes(){ }
//
//    void add(T);
//
//    const void* getAttributes() const;
//  std::vector<T> getAttributeContainer();
//    BufferType getAttributeType() const;
//    const DataFormatBase* getDataFormat() const;
//    unsigned int getElementSize() const;
//    unsigned int getNumberOfAttributes() const;
//    unsigned int getDataSize() const;
//
//private:
//    const DataFormat<T,B>* dataFormat_;
//    std::vector<T> attributes_;
//};
//
//template<typename T, size_t B, BufferType A>
//void Attributes<T,B,A>::add(T v){
//    attributes_.push_back(v);
//}
//
//template<typename T, size_t B, BufferType A>
//const void* Attributes<T,B,A>::getAttributes() const{
//    return static_cast<const void*>(&attributes_[0]);
//}
//
//template<typename T, size_t B, BufferType A>
//std::vector<T> Attributes<T,B,A>::getAttributeContainer() {
//    return attributes_;
//}
//
//template<typename T, size_t B, BufferType A>
//BufferType Attributes<T,B,A>::getAttributeType() const{
//    return A;
//}
//
//template<typename T, size_t B, BufferType A>
//const DataFormatBase* Attributes<T,B,A>::getDataFormat() const{
//    return dataFormat_;
//}
//
//template<typename T, size_t B, BufferType A>
//unsigned int Attributes<T,B,A>::getElementSize() const{
//    return sizeof(T);
//}
//
//template<typename T, size_t B, BufferType A>
//unsigned int Attributes<T,B,A>::getNumberOfAttributes() const{
//    return static_cast<unsigned int>(attributes_.size());
//}
//
//template<typename T, size_t B, BufferType A>
//unsigned int Attributes<T,B,A>::getDataSize() const{
//    return getElementSize()*getNumberOfAttributes();
//}


} // namespace

#endif // IVW_ATTRIBUTES_H
