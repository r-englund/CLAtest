#ifndef IVW_BUFFER_RAM_H
#define IVW_BUFFER_RAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>
#include <inviwo/core/util/formats.h>

namespace inviwo {

class IVW_CORE_API BufferRAM : public BufferRepresentation {

public:
    BufferRAM(size_t size, BufferType type = POSITION_ATTRIB, const DataFormatBase* format = DataFormatBase::get());

    virtual ~BufferRAM();

    virtual void initialize();
    virtual void deinitialize();
    DataRepresentation* clone() const = 0;
    virtual std::string getClassName() const { return "BufferRAM"; }

    virtual void setSize(size_t size) {  size_ = size; deinitialize(); initialize(); }
    virtual void resize(size_t size);

    virtual void* getData() = 0; 
    virtual const void* getData() const = 0;
    //virtual void* getData() {return data_;};
    //virtual const void* getData() const {return data_;};

    virtual void setValueFromSingleFloat(size_t index, float val) = 0;
    virtual void setValueFromVec2Float(size_t index, vec2 val) = 0;
    virtual void setValueFromVec3Float(size_t index, vec3 val) = 0;
    virtual void setValueFromVec4Float(size_t index, vec4 val) = 0;

    virtual float getValueAsSingleFloat(size_t index) const = 0;
    virtual vec2 getValueAsVec2Float(size_t index) const = 0;
    virtual vec3 getValueAsVec3Float(size_t index) const = 0;
    virtual vec4 getValueAsVec4Float(size_t index) const = 0;

    // Takes ownership of data pointer
    //void setData(void* data) {
    //    deinitialize();
    //    data_ = data;
    //}
    
protected:
    //void* data_;
};

/**
 * Factory for buffers. 
 * Creates a BufferRAM with data type specified by format. 
 * 
 * @param size of buffer to create.
 * @param format of buffer to create.
 * @return NULL if no valid format was specified. 
 */
IVW_CORE_API BufferRAM* createBufferRAM(size_t size, BufferType type, const DataFormatBase* format);

} // namespace

#endif // IVW_BUFFER_RAM_H
