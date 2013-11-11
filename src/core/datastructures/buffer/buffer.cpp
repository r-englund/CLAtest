#include <inviwo/core/datastructures/buffer/buffer.h>
#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>
#include <inviwo/core/datastructures/buffer/bufferram.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>

namespace inviwo {


Buffer::Buffer(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage): 
    Data(), size_(size), format_(format), type_(type), usage_(usage){
}
Buffer::Buffer(const Buffer& rhs) : Data(rhs), size_(rhs.size_), format_(rhs.format_), type_(rhs.type_), usage_(rhs.usage_) {

}
Buffer::~Buffer(){

}

void Buffer::resize( size_t size ){
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

size_t Buffer::getSizeInBytes(){
    return size_ * format_->getBytesStored();
}

void Buffer::setSize( size_t size ){
    resize(size);
}

Buffer* Buffer::clone() const{
    return new Buffer(*this);
}

DataRepresentation* Buffer::createDefaultRepresentation() {
    return createBufferRAM(getSize(), format_, type_, usage_);
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

