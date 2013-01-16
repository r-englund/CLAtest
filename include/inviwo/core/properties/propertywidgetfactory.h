#ifndef IVW_PROPERTYWIDGETFACTORY_H
#define IVW_PROPERTYWIDGETFACTORY_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/properties/propertywidget.h"
#include "inviwo/core/inviwofactorybase.h"

namespace inviwo {

    class IVW_CORE_API PropertyWidgetFactory : public InviwoFactoryBase {

public:
    PropertyWidgetFactory();
    ~PropertyWidgetFactory();

    static PropertyWidgetFactory* instance();

    virtual void initialize();
    virtual void deinitialize();

    void registerPropertyWidget(PropertyWidget* propertyWidget);
    virtual IvwSerializable* create(std::string className) const;
    virtual bool isValidType(std::string className) const;

protected:
    static PropertyWidgetFactory* factory_;

private:
    mutable std::map<std::string, PropertyWidget*> propertyWidgetMap_;
};

} // namespace

#endif // IVW_PROPERTYWIDGETFACTORY_H
