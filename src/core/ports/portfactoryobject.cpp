#include <inviwo/core/ports/portfactoryobject.h>

namespace inviwo {

    PortFactoryObject::PortFactoryObject(const std::string& className)
        : className_(className) {}

    PortFactoryObject::~PortFactoryObject() {}

    std::string PortFactoryObject::getClassName() const {
        return className_;
    }

} // namespace

