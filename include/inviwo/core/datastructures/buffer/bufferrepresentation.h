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
    BufferRepresentation(size_t size, const DataFormatBase* format);
    virtual ~BufferRepresentation() {};
    virtual void performOperation(DataOperation*) const {};
    virtual void resize(size_t size) { size_ = size; }
    size_t getSize() const { return size_; }
    virtual DataRepresentation* clone() const = 0;
    virtual std::string getClassName() const { return "BufferRepresentation"; }

protected:
    size_t size_;

};

} // namespace

#endif // IVW_BUFFER_REPRESENTATION_H
