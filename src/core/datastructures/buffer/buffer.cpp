#include <inviwo/core/datastructures/buffer/buffer.h>
#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>
#include <inviwo/core/datastructures/buffer/bufferram.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>

namespace inviwo {


Buffer::Buffer( size_t size, BufferType type, const DataFormatBase* format /*= DataFormatBase::get()*/ ): size_(size), type_(type), format_(format)
{

}

Buffer::~Buffer()
{

}

void Buffer::resize( size_t size )
{
    if(size != size_) {
        size_ = size;
        for (size_t i=0; i<representations_.size(); i++) {
            // Static cast is ok because the representations must be BufferRepresentations.
            BufferRepresentation* bufferRepresentation = static_cast<BufferRepresentation*>(representations_[i]) ;
            bufferRepresentation->resize(size);
        }
        setAllRepresentationsAsInvalid();
    }
}

size_t Buffer::getSizeInBytes()
{
    return size_ * format_->getBytesStored();
}

void Buffer::setSize( size_t size )
{
    resize(size);
}

Data* Buffer::clone() const
{
    Buffer* buffer = new Buffer(getSize(), type_, getDataFormat());
    // TODO: Copy at least one representation
    return buffer;
}

DataRepresentation* Buffer::createDefaultRepresentation() 
{
    return createBufferRAM(getSize(), type_, format_);

}

size_t Buffer::getSize() const
{
    // We need to update the size if a representation has changed size
    if(lastValidRepresentation_) {
        const_cast<Buffer*>(this)->size_ = static_cast<const BufferRepresentation*>(lastValidRepresentation_)->getSize();
    }
    return size_;
}

}

