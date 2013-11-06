#include <modules/opencl/buffer/bufferclconverter.h>

namespace inviwo {

BufferRAM2CLConverter::BufferRAM2CLConverter()
: RepresentationConverterType<BufferCL>(){
}

BufferRAM2CLConverter::~BufferRAM2CLConverter() {}

DataRepresentation* BufferRAM2CLConverter::createFrom(const DataRepresentation* source) {     
    const BufferRAM* bufferRAM = static_cast<const BufferRAM*>(source);
    BufferCL* bufferCL = new BufferCL(bufferRAM->getSize(), bufferRAM->getBufferType(), bufferRAM->getDataFormat(), bufferRAM->getData());
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
    BufferRAM* dst = createBufferRAM(src->getSize(), src->getBufferType(), src->getDataFormat());
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