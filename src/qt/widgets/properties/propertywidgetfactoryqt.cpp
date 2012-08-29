#include "inviwo/qt/widgets/properties/propertywidgetfactoryqt.h"

#include "inviwo/core/properties/cameraproperty.h"
#include "inviwo/core/properties/fileproperty.h"
#include "inviwo/core/properties/scalarproperties.h"
#include "inviwo/core/properties/vectorproperties.h"

#include "inviwo/qt/widgets/properties/camerapropertywidgetqt.h"
#include "inviwo/qt/widgets/properties/filepropertywidgetqt.h"
#include "inviwo/qt/widgets/properties/floatpropertywidgetqt.h"
#include "inviwo/qt/widgets/properties/floatvec3propertywidgetqt.h"
#include "inviwo/qt/widgets/properties/intpropertywidgetqt.h"

namespace inviwo {

const std::string PropertyWidgetFactoryQt::logSource_ = "PropertyWidgetFactoryQt";

PropertyWidgetFactoryQt::PropertyWidgetFactoryQt() {}

PropertyWidgetQt* PropertyWidgetFactoryQt::create(Property* property) {
    if (dynamic_cast<CameraProperty*>(property))
        return new CameraPropertyWidgetQt(static_cast<CameraProperty*>(property));
    if (dynamic_cast<FileProperty*>(property))
        return new FilePropertyWidgetQt(static_cast<FileProperty*>(property));
    if (dynamic_cast<FloatProperty*>(property))
        return new FloatPropertyWidgetQt(static_cast<FloatProperty*>(property));
    if (dynamic_cast<FloatVec3Property*>(property))
        return new FloatVec3PropertyWidgetQt(static_cast<FloatVec3Property*>(property));
    if (dynamic_cast<IntProperty*>(property))
        return new IntPropertyWidgetQt(static_cast<IntProperty*>(property));

    LogWarn("No widget for property " + property->getIdentifier() + " found.")
    return 0;
}

} // namespace
