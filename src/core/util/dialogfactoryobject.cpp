#include <inviwo/core/util/dialogfactoryobject.h>

namespace inviwo {

DialogFactoryObject::DialogFactoryObject(const std::string& className)
    : className_(className) {        
}

std::string DialogFactoryObject::getClassName() const {
    return className_;
}

} // namespace

