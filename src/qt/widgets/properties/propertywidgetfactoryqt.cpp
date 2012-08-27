#include "inviwo/qt/widgets/properties/propertywidgetfactoryqt.h"

#include "inviwo/core/properties/scalarproperties.h"
#include "inviwo/core/properties/vectorproperties.h"

#include "inviwo/qt/widgets/properties/floatpropertywidgetqt.h"

namespace inviwo {

PropertyWidgetFactoryQt::PropertyWidgetFactoryQt() {}

PropertyWidgetQt* PropertyWidgetFactoryQt::create(Property* property) {
    if (dynamic_cast<FloatProperty*>(property))
        return new FloatPropertyWidgetQt(static_cast<FloatProperty*>(property));
    return 0;
}

} // namespace
