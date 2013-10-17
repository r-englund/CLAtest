#ifndef IVW_ATTRIBUTES_H
#define IVW_ATTRIBUTES_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <vector>

namespace inviwo {

enum AttributeType{
    COLOR_ATTRIB,
    CURVATURE_ATTRIB,
    INDEX_ATTRIB,
    NORMAL_ATTRIB,
    POSITION_ATTRIB,
    TEXCOORD_ATTRIB
};

class IVW_CORE_API AttributesBase {
public:
    AttributesBase(){}
    virtual ~AttributesBase(){}
    virtual AttributesBase* clone() const = 0;

    virtual const void* getAttributes() const = 0;  
    virtual AttributeType getAttributeType() const = 0;
    virtual const DataFormatBase* getDataFormat() const = 0;
    virtual unsigned int getElementSize() const = 0;
    virtual unsigned int getNumberOfAttributes() const = 0;
    virtual unsigned int getDataSize() const = 0;
};

template<typename T, size_t B, AttributeType A>
class IVW_CORE_API Attributes : public AttributesBase {

public:
    Attributes() : AttributesBase(), dataFormat_(DataFormat<T,B>::get()) {}
    virtual ~Attributes(){ }
    AttributesBase* clone() const;

    void add(T);
    
    const void* getAttributes() const;
    const std::vector<T>& getAttributeContainer() const;
    AttributeType getAttributeType() const;
    const DataFormatBase* getDataFormat() const;
    unsigned int getElementSize() const;
    unsigned int getNumberOfAttributes() const;
    unsigned int getDataSize() const;
    
private:
    const DataFormat<T,B>* dataFormat_;
    std::vector<T> attributes_;
};

template<typename T, size_t B, AttributeType A>
AttributesBase* Attributes<T,B,A>::clone() const{
    Attributes<T,B,A>* newAttributes = new Attributes<T,B,A>();
    for(typename std::vector<T>::const_iterator it = getAttributeContainer().begin(); it != getAttributeContainer().end(); ++it)
        newAttributes->add(*it);
    return newAttributes;
}

template<typename T, size_t B, AttributeType A>
void Attributes<T,B,A>::add(T v){
    attributes_.push_back(v);
}

template<typename T, size_t B, AttributeType A>
const void* Attributes<T,B,A>::getAttributes() const{
    if(!attributes_.empty())
        return static_cast<const void*>(&attributes_[0]);
    else
        return NULL;
}

template<typename T, size_t B, AttributeType A>
const std::vector<T>& Attributes<T,B,A>::getAttributeContainer() const {
    return attributes_;
}

template<typename T, size_t B, AttributeType A>
AttributeType Attributes<T,B,A>::getAttributeType() const{
    return A;
}

template<typename T, size_t B, AttributeType A>
const DataFormatBase* Attributes<T,B,A>::getDataFormat() const{
    return dataFormat_;
}

template<typename T, size_t B, AttributeType A>
unsigned int Attributes<T,B,A>::getElementSize() const{
    return sizeof(T);
}

template<typename T, size_t B, AttributeType A>
unsigned int Attributes<T,B,A>::getNumberOfAttributes() const{
    return static_cast<unsigned int>(attributes_.size());
}

template<typename T, size_t B, AttributeType A>
unsigned int Attributes<T,B,A>::getDataSize() const{
    return getElementSize()*getNumberOfAttributes();
}

#define DataFormatAttributes(D, A) Attributes<D::type, D::bits, A>

typedef DataFormatAttributes(DataVec4FLOAT32, COLOR_ATTRIB) ColorAttributes;
typedef DataFormatAttributes(DataFLOAT32, CURVATURE_ATTRIB) CurvatureAttributes;
typedef DataFormatAttributes(DataUINT32, INDEX_ATTRIB) IndexAttributes;
typedef DataFormatAttributes(DataVec2FLOAT32, POSITION_ATTRIB) Position2dAttributes;
typedef DataFormatAttributes(DataVec2FLOAT32, TEXCOORD_ATTRIB) TexCoord2dAttributes;
typedef DataFormatAttributes(DataVec3FLOAT32, POSITION_ATTRIB) Position3dAttributes;
typedef DataFormatAttributes(DataVec3FLOAT32, TEXCOORD_ATTRIB) TexCoord3dAttributes;
typedef DataFormatAttributes(DataVec3FLOAT32, NORMAL_ATTRIB) NormalAttributes;

} // namespace

#endif // IVW_ATTRIBUTES_H
