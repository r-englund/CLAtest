#include <modules/opencl/buffer/bufferclconverter.h>
#include <modules/opencl/buffer/bufferclglconverter.h>
#include <modules/opencl/syncclgl.h>
#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

BufferRAM2CLGLConverter::BufferRAM2CLGLConverter()
    : RepresentationConverterPackage<BufferCLGL>()
{
    addConverter(new BufferRAM2GLConverter());
    addConverter(new BufferGL2CLGLConverter());
}

BufferCLGL2RAMConverter::BufferCLGL2RAMConverter()
    : RepresentationConverterType<BufferRAM>()
{
}


DataRepresentation* BufferCLGL2RAMConverter::createFrom(const DataRepresentation* source) {     
    DataRepresentation* destination = 0;
    const BufferCLGL* src = static_cast<const BufferCLGL*>(source);
    size_t size = src->getSize();
    destination = createBufferRAM(size, src->getBufferType(), src->getDataFormat()); 
    const BufferGL* buffer = src->getBufferGL();
    if (destination) {
        BufferRAM* dst = static_cast<BufferRAM*>(destination);
        buffer->download(dst->getData());
    } else {
        LogError("Invalid conversion or not implemented");
    }
    return destination;
}

void BufferCLGL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const BufferCLGL* src = static_cast<const BufferCLGL*>(source);
    BufferRAM* dst = static_cast<BufferRAM*>(destination);
    if(src->getSize() != dst->getSize()) {
        dst->setSize(src->getSize());
    }
    src->getBufferGL()->download(dst->getData());
}


DataRepresentation* BufferGL2CLGLConverter::createFrom(const DataRepresentation* source )
{
    DataRepresentation* destination = 0;
    const BufferGL* src = static_cast<const BufferGL*>(source);
    destination = new BufferCLGL(src->getSize(), src->getBufferType(), src->getDataFormat(), src);
    return destination;
}

void BufferGL2CLGLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {

    const BufferGL* src = static_cast<const BufferGL*>(source);
    BufferCLGL* dst = static_cast<BufferCLGL*>(destination);
    if(src->getSize() != dst->getSize()) {
        dst->setSize(src->getSize());
    }
}


DataRepresentation* BufferCLGL2CLConverter::createFrom(const DataRepresentation* source )
{
    DataRepresentation* destination = 0;
    const BufferCLGL* src = static_cast<const BufferCLGL*>(source);
    size_t size = src->getSize();;
    destination = new BufferCL(size, src->getBufferType(), src->getDataFormat());
    SyncCLGL glSync;
    src->aquireGLObject(glSync.getGLSyncEvent());
    OpenCL::instance()->getQueue().enqueueCopyBuffer(src->getBuffer(), static_cast<BufferCL*>(destination)->getBuffer(), 0, 0, size);
    src->releaseGLObject(glSync.getGLSyncEvent());

    return destination;
}
void BufferCLGL2CLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const BufferCLGL* src = static_cast<const BufferCLGL*>(source);
    BufferCL* dst = static_cast<BufferCL*>(destination);
    if(src->getSize() != dst->getSize()) {
        dst->setSize(src->getSize());
    }
    SyncCLGL glSync;
    src->aquireGLObject(glSync.getGLSyncEvent());
    OpenCL::instance()->getQueue().enqueueCopyBuffer(src->getBuffer(), dst->getBuffer(), 0, 0, src->getSize());
    src->releaseGLObject(glSync.getGLSyncEvent());

}



BufferCL2CLGLConverter::BufferCL2CLGLConverter() : RepresentationConverterPackage<BufferCLGL>()
{
    addConverter(new BufferCL2RAMConverter());
    addConverter(new BufferRAM2GLConverter());
    addConverter(new BufferGL2CLGLConverter());
}

} // namespace
