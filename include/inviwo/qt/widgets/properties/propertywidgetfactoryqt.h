#ifndef IVW_PROPERTYWIDGETFACTORYQT_H
#define IVW_PROPERTYWIDGETFACTORYQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/core/util/singleton.h>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

namespace inviwo {

class IVW_QTWIDGETS_API PropertyWidgetFactoryQt : public Singleton<PropertyWidgetFactoryQt> {
//TODO: derive from Factory class

public:
    PropertyWidgetFactoryQt();
    PropertyWidgetQt* create(Property* property);
};

} // namespace

#endif // IVW_PROPERTYWIDGETFACTORYQT_H