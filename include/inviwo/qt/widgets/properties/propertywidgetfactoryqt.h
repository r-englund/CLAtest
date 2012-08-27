#ifndef IVW_PROPERTYWIDGETFACTORYQT_H
#define IVW_PROPERTYWIDGETFACTORYQT_H

#include "inviwo/core/properties/property.h"

#include "inviwo/qt/widgets/properties/propertywidgetqt.h"

namespace inviwo {

class PropertyWidgetFactoryQt {

public:
    PropertyWidgetFactoryQt();
    PropertyWidgetQt* create(Property* property);
};

} // namespace

#endif // IVW_PROPERTYWIDGETFACTORYQT_H