#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>


inviwo::BufferRepresentation::BufferRepresentation( size_t size, BufferType type, const DataFormatBase* format )
    : DataRepresentation(format), size_(size), type_(type)
{

}
