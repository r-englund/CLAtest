#include <inviwo/core/datastructures/buffer/bufferram.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>

namespace inviwo {

BufferRAM::BufferRAM( size_t size, BufferType type, const DataFormatBase* format /*= DataFormatBase::get()*/ )
: BufferRepresentation(size, type, format)
{
    BufferRAM::initialize();
}

BufferRAM::~BufferRAM() {
    deinitialize();

}

void BufferRAM::initialize()
{
    //data_ = NULL;
}

void BufferRAM::deinitialize()
{
    // Make sure that data is deinitialized in
    // child class (should not delete void pointer 
    // since destructor will not be called for object).
}

void BufferRAM::resize( size_t size )
{
    size_ = size;
    //Delete and reallocate data_ to new size
    BufferRAM::deinitialize();
    initialize();
}

IVW_CORE_API BufferRAM* createBufferRAM( size_t size, BufferType type, const DataFormatBase* format )
{
    switch (format->getId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("createBufferRAM", "Invalid format");
        return NULL;
        #define DataFormatIdMacro(i) case i: return new BufferRAMCustomPrecision<Data##i::type, Data##i::bits>(size, type, format); break;
        #include <inviwo/core/util/formatsdefinefunc.h>
    default:
        LogErrorCustom("createBufferRAM", "Invalid format or not implemented");
        return NULL;
    }
    return NULL;
}


}
