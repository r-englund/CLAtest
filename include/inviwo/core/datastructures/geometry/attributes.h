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

template<typename T, AttributeType A>
class IVW_CORE_API Attributes {

public:
    Attributes(){}
    virtual ~Attributes(){}
    
    T* getAttributes() const;
    
private:
    std::vector<T> attributes_;
};

template<typename T, AttributeType A>
T* Attributes<T,A>::getAttributes() const{
    &attributes_[0];
}

} // namespace

typedef Attributes<glm::vec3, POSITION> VertexPositions;
typedef Attributes<glm::vec3, COLOR> VertexColors;

#endif // IVW_ATTRIBUTES_H
