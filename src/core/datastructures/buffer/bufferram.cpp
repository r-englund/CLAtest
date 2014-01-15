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

#include <inviwo/core/datastructures/buffer/bufferram.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>

namespace inviwo {

BufferRAM::BufferRAM(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage)
    : BufferRepresentation(size, format, type, usage){
    BufferRAM::initialize();
}
BufferRAM::BufferRAM(const BufferRAM& rhs) : BufferRepresentation(rhs){
    BufferRAM::initialize();
} 
BufferRAM& BufferRAM::operator=(const BufferRAM& that) {
    if(this != &that) {
        deinitialize();
        BufferRepresentation::operator=(that);
        initialize();
    }
    return *this;
}
BufferRAM::~BufferRAM() {
    deinitialize();
}

void BufferRAM::initialize(){
}

void BufferRAM::deinitialize(){
    // Make sure that data is deinitialized in
    // child class (should not delete void pointer 
    // since destructor will not be called for object).
}

void BufferRAM::setSize(size_t size) {  
    size_ = size; 
    deinitialize(); 
    initialize(); 
}

void BufferRAM::resize( size_t size ){
    size_ = size;
    //Delete and reallocate data_ to new size
    BufferRAM::deinitialize();
    initialize();
}

BufferRAM* createBufferRAM(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage){
    switch (format->getId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("createBufferRAM", "Invalid format");
        return NULL;
        #define DataFormatIdMacro(i) case i: return new BufferRAMCustomPrecision<Data##i::type, Data##i::bits>(size, format, type, usage); break;
        #include <inviwo/core/util/formatsdefinefunc.h>
    default:
        LogErrorCustom("createBufferRAM", "Invalid format or not implemented");
        return NULL;
    }
    return NULL;
}


}
