#ifndef IVW_PROPERTYWIDGETFACTORY_H
#define IVW_PROPERTYWIDGETFACTORY_H

#include "inviwo/core/properties/propertywidget.h"

namespace inviwo {

class PropertyWidgetFactory {

public:
    PropertyWidgetFactory();
    ~PropertyWidgetFactory();

    static PropertyWidgetFactory* instance();

    void initialize();
    void deinitialize();

    void registerPropertyWidget(PropertyWidget* propertyWidget);
    PropertyWidget* create(std::string className) const;

protected:
    static PropertyWidgetFactory* factory_;

private:
    mutable std::map<std::string, PropertyWidget*> propertyWidgetMap_;
};

} // namespace

#endif // IVW_PROPERTYWIDGETFACTORY_H
