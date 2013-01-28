#ifndef IVW_PROPERTYWIDGETFACTORY_H
#define IVW_PROPERTYWIDGETFACTORY_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/properties/propertywidget.h>
#include <inviwo/core/util/inviwofactorybase.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_CORE_API PropertyWidgetFactory : public Factory,
                                           public Singleton<PropertyWidgetFactory> {

public:
    PropertyWidgetFactory();
    ~PropertyWidgetFactory();

    virtual void initialize();
    virtual void deinitialize();

    void registerPropertyWidget(PropertyWidget* propertyWidget);
    virtual IvwSerializable* create(std::string className) const;
    virtual bool isValidType(std::string className) const;

private:
    mutable std::map<std::string, PropertyWidget*> propertyWidgetMap_;
};

} // namespace

#endif // IVW_PROPERTYWIDGETFACTORY_H
