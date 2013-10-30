#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>

namespace inviwo {

BufferRepresentation::BufferRepresentation( size_t size, BufferType type, const DataFormatBase* format )
    : DataRepresentation(format), size_(size), type_(type){
}

BufferRepresentation::BufferRepresentation(const BufferRepresentation& rhs) 
    : DataRepresentation(rhs), size_(rhs.size_), type_(rhs.type_){
}


} //namespace