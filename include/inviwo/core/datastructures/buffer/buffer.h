#ifndef IVW_BUFFER_H
#define IVW_BUFFER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>


namespace inviwo {

enum BufferType {
    COLOR_ATTRIB,
    CURVATURE_ATTRIB,
    INDEX_ATTRIB,
    NORMAL_ATTRIB,
    POSITION_ATTRIB,
    TEXCOORD_ATTRIB
};

enum BufferUsage {
    STATIC,
    DYNAMIC
};

class IVW_CORE_API Buffer : public Data {

public:
    Buffer(size_t size,  const DataFormatBase* format = DataFormatBase::get(), BufferType type = POSITION_ATTRIB, BufferUsage usage = STATIC);
    Buffer(const Buffer& rhs);
    virtual ~Buffer();
    void resize(size_t size);
    virtual Buffer* clone() const;
    void resizeBufferRepresentations(Buffer* targetBuffer, size_t targetSize);

    size_t getSize() const;
	void setSize(size_t size);

    size_t getSizeInBytes();
    BufferType getBufferType() const { return type_; }

protected:
    virtual DataRepresentation* createDefaultRepresentation();
private:
    size_t size_;
    const DataFormatBase* format_;
    BufferType type_;
    BufferUsage usage_;
};

template<typename T, size_t B, BufferType A>
class Attributes : public Buffer {

public:
    Attributes(size_t size = 0, BufferUsage usage = STATIC): Buffer(size, DataFormat<T,B>::get(), A, usage) {}
    Attributes(BufferUsage usage): Buffer(0, DataFormat<T,B>::get(), A, usage) {}

    virtual ~Attributes(){ }

    virtual Attributes* clone() const { return new Attributes(*this); }

private:
    static const DataFormatBase* defaultformat(){
        return  DataFormat<T, B>::get();
    }

};

#define DataFormatBuffers(D, A) Attributes<D::type, D::bits, A>

typedef DataFormatBuffers(DataVec4FLOAT32, COLOR_ATTRIB) ColorBuffer;
typedef DataFormatBuffers(DataFLOAT32, CURVATURE_ATTRIB) CurvatureBuffer;
typedef DataFormatBuffers(DataUINT32, INDEX_ATTRIB) IndexBuffer;
typedef DataFormatBuffers(DataVec2FLOAT32, POSITION_ATTRIB) Position2dBuffer;
typedef DataFormatBuffers(DataVec2FLOAT32, TEXCOORD_ATTRIB) TexCoord2dBuffer;
typedef DataFormatBuffers(DataVec3FLOAT32, POSITION_ATTRIB) Position3dBuffer;
typedef DataFormatBuffers(DataVec3FLOAT32, TEXCOORD_ATTRIB) TexCoord3dBuffer;
typedef DataFormatBuffers(DataVec3FLOAT32, NORMAL_ATTRIB) NormalBuffer;

#define DataFormatIdMacro(i) typedef Attributes<Data##i::type, Data##i::bits, POSITION_ATTRIB> Buffer_##i;
#include <inviwo/core/util/formatsdefinefunc.h>


} // namespace

#endif // IVW_BUFFER_H
