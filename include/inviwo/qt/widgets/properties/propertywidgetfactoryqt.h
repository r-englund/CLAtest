#ifndef IVW_PROPERTYWIDGETFACTORYQT_H
#define IVW_PROPERTYWIDGETFACTORYQT_H

#include "inviwo/qt/widgets/inviwoqtwidgetsdefine.h"
#include "inviwo/core/properties/property.h"

#include "inviwo/qt/widgets/properties/propertywidgetqt.h"

namespace inviwo {

class IVW_QTWIDGETS_API PropertyWidgetFactoryQt {

public:
    PropertyWidgetFactoryQt();
    PropertyWidgetQt* create(Property* property);

    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_PROPERTYWIDGETFACTORYQT_H