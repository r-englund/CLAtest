#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>


inviwo::BufferRepresentation::BufferRepresentation( size_t size, const DataFormatBase* format )
    : DataRepresentation(format), size_(size)
{

}
