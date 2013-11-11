#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>

namespace inviwo {

BufferRepresentation::BufferRepresentation(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage)
    : DataRepresentation(format), size_(size), type_(type), usage_(usage){
}

BufferRepresentation::BufferRepresentation(const BufferRepresentation& rhs) 
    : DataRepresentation(rhs), size_(rhs.size_), type_(rhs.type_), usage_(rhs.usage_){
}


} //namespace