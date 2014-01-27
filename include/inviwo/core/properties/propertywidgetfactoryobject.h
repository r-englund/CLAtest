#ifndef IVW_PROPERTYWIDGETFACTORYOBJECT_H
#define IVW_PROPERTYWIDGETFACTORYOBJECT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/propertysemantics.h>
#include <string>

namespace inviwo {

class Property;
class PropertyWidget;

class IVW_CORE_API PropertyWidgetFactoryObject {
public:
    PropertyWidgetFactoryObject(const std::string& className, const PropertySemantics& semantics);
    virtual ~PropertyWidgetFactoryObject();

    virtual PropertyWidget* create(Property*) = 0;

    std::string getClassName() const;
    PropertySemantics getSematics() const;

private:
    std::string className_;
    PropertySemantics semantics_;
};

template<typename T, typename P>
class PropertyWidgetFactoryObjectTemplate : public PropertyWidgetFactoryObject {
public:
    PropertyWidgetFactoryObjectTemplate(const std::string& className, const PropertySemantics& semantics)
        : PropertyWidgetFactoryObject(className, semantics) {}

    virtual ~PropertyWidgetFactoryObjectTemplate() {}

    virtual PropertyWidget* create(Property* prop) {
        return static_cast<PropertyWidget*>(new T(static_cast<P*>(prop)));
    }
};

} // namespace

#endif // IVW_PROPERTYWIDGETFACTORYOBJECT_H

