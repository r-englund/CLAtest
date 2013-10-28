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
class IVW_CORE_API Buffer : public Data {

public:
    Buffer(size_t size, BufferType type = POSITION_ATTRIB, const DataFormatBase* format = DataFormatBase::get());
    virtual ~Buffer();
    void resize(size_t size);
    virtual Data* clone() const;
    void resizeBufferRepresentations(Buffer* targetBuffer, size_t targetSize);

    size_t getSize() const;
	void setSize(size_t size);

    size_t getSizeInBytes();
    BufferType getBufferType() const { return type_; }

protected:
    void createDefaultRepresentation();
private:
    size_t size_;
    const DataFormatBase* format_;
    BufferType type_;
};

template<typename T, size_t B, BufferType A>
class Attributes : public Buffer {

public:
    Attributes() : Buffer(0, A, DataFormat<T,B>::get()) {}
    virtual ~Attributes(){ }

    virtual Data* clone() const { return new Attributes(*this); }

    std::vector<T> getAttributeContainer() const { return this->getRepresentation<T>()->getDataContainer(); }

private:

};


#define DataFormatBuffers(D, A) Attributes<D::type, D::bits, A>

//typedef DataVec4FLOAT32 ColorBuffer;
//typedef DataFLOAT32 CurvatureBuffer;
//typedef DataUINT32 IndexBuffer;
//typedef DataVec2FLOAT32 Position2dBuffer;
//typedef DataVec2FLOAT32 TexCoord2dBuffer;
//typedef DataVec3FLOAT32 Position3dBuffer;
//typedef DataVec3FLOAT32 TexCoord3dBuffer;
//typedef DataVec3FLOAT32 NormalBuffer;

typedef DataFormatBuffers(DataVec4FLOAT32, COLOR_ATTRIB) ColorBuffer;
typedef DataFormatBuffers(DataFLOAT32, CURVATURE_ATTRIB) CurvatureBuffer;
typedef DataFormatBuffers(DataUINT32, INDEX_ATTRIB) IndexBuffer;
typedef DataFormatBuffers(DataVec2FLOAT32, POSITION_ATTRIB) Position2dBuffer;
typedef DataFormatBuffers(DataVec2FLOAT32, TEXCOORD_ATTRIB) TexCoord2dBuffer;
typedef DataFormatBuffers(DataVec3FLOAT32, POSITION_ATTRIB) Position3dBuffer;
typedef DataFormatBuffers(DataVec3FLOAT32, TEXCOORD_ATTRIB) TexCoord3dBuffer;
typedef DataFormatBuffers(DataVec3FLOAT32, NORMAL_ATTRIB) NormalBuffer;




} // namespace

#endif // IVW_BUFFER_H
