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

#include <modules/opencl/buffer/bufferclconverter.h>

namespace inviwo {

BufferRAM2CLConverter::BufferRAM2CLConverter()
: RepresentationConverterType<BufferCL>(){
}

BufferRAM2CLConverter::~BufferRAM2CLConverter() {}

DataRepresentation* BufferRAM2CLConverter::createFrom(const DataRepresentation* source) {     
    const BufferRAM* bufferRAM = static_cast<const BufferRAM*>(source);
    BufferCL* bufferCL = new BufferCL(bufferRAM->getSize(), bufferRAM->getDataFormat(), bufferRAM->getBufferType(), bufferRAM->getBufferUsage(), bufferRAM->getData());
    return bufferCL;
}
void BufferRAM2CLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const BufferRAM* src = static_cast<const BufferRAM*>(source);
    BufferCL* dst = static_cast<BufferCL*>(destination);
    dst->upload(src->getData(), src->getSize()*src->getSizeOfElement());

}



BufferCL2RAMConverter::BufferCL2RAMConverter()
: RepresentationConverterType<BufferRAM>(){
}

BufferCL2RAMConverter::~BufferCL2RAMConverter() {}

DataRepresentation* BufferCL2RAMConverter::createFrom(const DataRepresentation* source) {     
    const BufferCL* src = static_cast<const BufferCL*>(source);
    BufferRAM* dst = createBufferRAM(src->getSize(), src->getDataFormat(), src->getBufferType(), src->getBufferUsage());
    src->download(dst->getData());
    return dst;
}

void BufferCL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const BufferCL* src = static_cast<const BufferCL*>(source);
    BufferRAM* dst = static_cast<BufferRAM*>(destination);
    // TODO: Take care of different sizes
    src->download(dst->getData());

}

} // end namespace