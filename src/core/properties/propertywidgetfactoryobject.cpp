#include <inviwo/core/properties/propertywidgetfactoryobject.h>

namespace inviwo {
PropertyWidgetFactoryObject::PropertyWidgetFactoryObject(const std::string& className,
                                                            const PropertySemantics& semantics)
    : className_(className), semantics_(semantics) {
}

PropertyWidgetFactoryObject::~PropertyWidgetFactoryObject() {}

std::string PropertyWidgetFactoryObject::getClassName() const {
    return className_;
}

PropertySemantics PropertyWidgetFactoryObject::getSematics() const {
    return semantics_;
}

} // namespace

