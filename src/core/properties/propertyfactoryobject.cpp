#include <inviwo/core/properties/propertyfactoryobject.h>

namespace inviwo {

PropertyFactoryObject::PropertyFactoryObject(const std::string& className)
    : className_(className) {}

PropertyFactoryObject::~PropertyFactoryObject() {}

std::string PropertyFactoryObject::getClassName() const {
    return className_;
}

} // namespace

