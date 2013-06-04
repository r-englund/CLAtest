#ifndef IVW_ATTRIBUTES_H
#define IVW_ATTRIBUTES_H

#include <inviwo/core/common/inviwocoredefine.h>

namespace inviwo {

enum AttributeType{
    POSITION,
    NORMAL,
    COLOR,
    CURVATURE
};

class IVW_CORE_API AttributesBase {
public:
    AttributesBase(){}
    virtual ~AttributesBase(){}

    virtual void* getAttributes() const = 0;  
    virtual AttributeType getAttributeType() const = 0;
};

template<typename T, AttributeType A>
class IVW_CORE_API Attributes : public AttributesBase {

public:
    Attributes() : AttributesBase() {}
    virtual ~Attributes(){}
    
    void* getAttributes() const;
    AttributeType getAttributeType() const;
    
private:
    std::vector<T> attributes_;
};

template<typename T, AttributeType A>
void* Attributes<T,A>::getAttributes() const{
    &attributes_[0];
}

template<typename T, AttributeType A>
AttributeType Attributes<T,A>::getAttributeType() const{
    return A;
}

} // namespace

typedef Attributes<glm::vec3, AttributeType::POSITION> PositionAttributes;
typedef Attributes<glm::vec3, AttributeType::COLOR> ColorAttributes;
typedef Attributes<glm::vec3, AttributeType::NORMAL> NormalAttribute;
typedef Attributes<float, AttributeType::CURVATURE> CurvatureAttributes;

#endif // IVW_ATTRIBUTES_H
