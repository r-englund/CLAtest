#ifndef IVW_BUFFER_H
#define IVW_BUFFER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>


namespace inviwo {

class IVW_CORE_API Buffer : public Data {

public:
    Buffer(size_t size, const DataFormatBase* format = DataFormatBase::get());
    virtual ~Buffer();
    void resize(size_t size);
    virtual Data* clone() const;
    void resizeBufferRepresentations(Buffer* targetBuffer, size_t targetSize);

	size_t getSize() const;
	void setSize(size_t size);

    size_t getSizeInBytes();

protected:
    void createDefaultRepresentation() const;
    void newEditableRepresentationCreated() const;
private:
    size_t size_;
    const DataFormatBase* format_;
};

} // namespace

#endif // IVW_BUFFER_H
