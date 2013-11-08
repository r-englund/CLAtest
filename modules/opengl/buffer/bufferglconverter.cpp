#include <modules/opengl/buffer/bufferglconverter.h>

namespace inviwo {

BufferRAM2GLConverter::BufferRAM2GLConverter()
    : RepresentationConverterType<BufferGL>(){
}

BufferRAM2GLConverter::~BufferRAM2GLConverter() {}

DataRepresentation* BufferRAM2GLConverter::createFrom(const DataRepresentation* source) {     
    const BufferRAM* bufferRAM = static_cast<const BufferRAM*>(source);
    BufferGL* bufferGL = new BufferGL(bufferRAM->getSize(), bufferRAM->getBufferType(), bufferRAM->getDataFormat());
    bufferGL->upload(bufferRAM->getData(), bufferRAM->getSize()*bufferRAM->getSizeOfElement());

    return bufferGL;
}
void BufferRAM2GLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const BufferRAM* src = static_cast<const BufferRAM*>(source);
    BufferGL* dst = static_cast<BufferGL*>(destination);
    dst->reupload(src->getData(), src->getSize());

}


BufferGL2RAMConverter::BufferGL2RAMConverter()
: RepresentationConverterType<BufferRAM>()
{}

BufferGL2RAMConverter::~BufferGL2RAMConverter() {}

DataRepresentation* BufferGL2RAMConverter::createFrom(const DataRepresentation* source) {
    const BufferGL* src = static_cast<const BufferGL*>(source);
    BufferRAM* dst = createBufferRAM(src->getSize(), src->getBufferType(), src->getDataFormat()); 
    if (dst) {
        src->download(dst->getData());

        return dst;
    } else {
        LogError("Cannot convert format from GL to RAM:" << src->getDataFormat()->getString());
    }
    return NULL;
}

void BufferGL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const BufferGL* src = static_cast<const BufferGL*>(source);
    BufferRAM* dst = static_cast<BufferRAM*>(destination);
    if(src->getSize() != dst->getSize()) {
        dst->resize(src->getSize());
    }
    src->download(dst->getData());
}


} // namespace