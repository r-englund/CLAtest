#include <inviwo/core/datastructures/buffer/buffer.h>
#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>


inviwo::Buffer::Buffer( size_t size, const DataFormatBase* format /*= DataFormatBase::get()*/ ): size_(size)
{

}

inviwo::Buffer::~Buffer()
{

}

void inviwo::Buffer::resize( size_t size )
{
    setSize(size);
    for (size_t i=0; i<representations_.size(); i++) {
        // Static cast is ok because the representations must be BufferRepresentations.
        BufferRepresentation* bufferRepresentation = static_cast<BufferRepresentation*>(representations_[i]) ;
        bufferRepresentation->resize(size);
    }
    setAllRepresentationsAsInvalid();
}


