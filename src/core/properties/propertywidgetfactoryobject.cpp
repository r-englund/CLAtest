#include <inviwo/core/properties/propertywidgetfactoryobject.h>

namespace inviwo {
    PropertyWidgetFactoryObject::PropertyWidgetFactoryObject(const std::string& className,
                                                             const std::string& semantics)
        : className_(className), semantics_(semantics) {
    }

    PropertyWidgetFactoryObject::~PropertyWidgetFactoryObject() {}

    std::string PropertyWidgetFactoryObject::getClassName() const {
        return className_;
    }

    std::string PropertyWidgetFactoryObject::getSematics() const {
        return semantics_;
    }

} // namespace

