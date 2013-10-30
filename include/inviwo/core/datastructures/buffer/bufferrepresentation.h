#ifndef IVW_BUFFER_REPRESENTATION_H
#define IVW_BUFFER_REPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/datarepresentation.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/buffer/buffer.h>

namespace inviwo {

class IVW_CORE_API BufferRepresentation : public DataRepresentation {

friend class Image;

public:
    BufferRepresentation(size_t size, BufferType type = POSITION_ATTRIB, const DataFormatBase* format = DataFormatBase::get());
    BufferRepresentation(const BufferRepresentation& rhs);
    virtual ~BufferRepresentation() {};
    virtual void performOperation(DataOperation*) const {};
    virtual void setSize(size_t size) {  size_ = size; }
    virtual void resize(size_t size) { size_ = size; }

    virtual BufferRepresentation* clone() const = 0;
    virtual std::string getClassName() const { return "BufferRepresentation"; }
    /**
     * Return the number of elements in the buffer.
     * 
     * @return Number of elements in the buffer
     */
    size_t getSize() const { return size_; }

    /**
     * Return size of buffer element in bytes.
     * 
     * @return Size of element in bytes.
     */
    virtual size_t getSizeOfElement() const { return getDataFormat()->getBytesStored(); };

    BufferType getBufferType() const { return type_; }
protected:
    size_t size_;
    BufferType type_;
};

} // namespace

#endif // IVW_BUFFER_REPRESENTATION_H
