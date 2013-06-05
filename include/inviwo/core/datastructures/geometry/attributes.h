#ifndef IVW_ATTRIBUTES_H
#define IVW_ATTRIBUTES_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <vector>

namespace inviwo {

enum AttributeType{
    COLOR,
    CURVATURE,
    INDEX,
    NORMAL,
    POSITION,
    TEXCOORD
};

class IVW_CORE_API AttributesBase {
public:
    AttributesBase(){}
    virtual ~AttributesBase(){}

    virtual const void* getAttributes() const = 0;  
    virtual AttributeType getAttributeType() const = 0;
    virtual unsigned int getElementSize() const = 0;
    virtual unsigned int getNumberOfAttributes() const = 0;
};

template<typename T, AttributeType A>
class IVW_CORE_API Attributes : public AttributesBase {

public:
    Attributes() : AttributesBase() {}
    virtual ~Attributes(){}

    void add(T&);
    
    const void* getAttributes() const;
    AttributeType getAttributeType() const;
    unsigned int getElementSize() const;
    unsigned int getNumberOfAttributes() const;
    
private:
    std::vector<T> attributes_;
};

template<typename T, AttributeType A>
void Attributes<T,A>::add(T& v){
    attributes_.push_back(v);
}

template<typename T, AttributeType A>
const void* Attributes<T,A>::getAttributes() const{
    return static_cast<const void*>(&attributes_[0]);
}

template<typename T, AttributeType A>
AttributeType Attributes<T,A>::getAttributeType() const{
    return A;
}

template<typename T, AttributeType A>
unsigned int Attributes<T,A>::getElementSize() const{
    return sizeof(T);
}

template<typename T, AttributeType A>
unsigned int Attributes<T,A>::getNumberOfAttributes() const{
    return attributes_.size();
}

typedef Attributes<glm::vec4, COLOR> ColorAttributes;
typedef Attributes<float, CURVATURE> CurvatureAttributes;
typedef Attributes<unsigned int, INDEX> IndexAttributes;
typedef Attributes<glm::vec3, NORMAL> NormalAttributes;
typedef Attributes<glm::vec3, POSITION> PositionAttributes;
typedef Attributes<glm::vec3, TEXCOORD> TexCoordAttributes;

} // namespace

#endif // IVW_ATTRIBUTES_H
