#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>

#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/properties/matrixproperties.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <inviwo/core/properties/snapshotproperty.h>
#include <inviwo/core/properties/stringproperty.h>
#include <inviwo/core/properties/stringoptionproperty.h>
#include <inviwo/core/properties/templatedoptionproperty.h>
#include <inviwo/core/properties/texteditorproperty.h>
#include <inviwo/core/properties/transferproperty.h>
#include <inviwo/core/properties/vectorproperties.h>

#include <inviwo/qt/widgets/properties/boolpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/buttonpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/camerapropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/compositepropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/filepropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatmat2propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatmat3propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatmat4propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatvec2propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatvec3propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatvec4propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/intpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/intvec2propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/intvec3propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/intvec4propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/snapshotpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/stringpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/texteditorwidgetqt.h>
#include <inviwo/qt/widgets/properties/transferpropertywidgetqt.h>


namespace inviwo {

    typedef FileProperty TextFileProperty;
    typedef StringProperty TextStringProperty;

const std::string PropertyWidgetFactoryQt::logSource_ = "PropertyWidgetFactoryQt";

PropertyWidgetFactoryQt::PropertyWidgetFactoryQt() {}
PropertyWidgetQt* PropertyWidgetFactoryQt::create(Property* property) {
    //if (dynamic_cast<CameraProperty*>(property))
    //    return new CameraPropertyWidgetQt(static_cast<CameraProperty*>(property));
    //if (dynamic_cast<TextFileProperty*>(property))
    //    return new TextEditorWidgetQt(static_cast<TextFileProperty*>(property),true);
    //if (dynamic_cast<TextStringProperty*>(property))
    //    return new TextEditorWidgetQt(static_cast<TextStringProperty*>(property),true);
    //if (dynamic_cast<TemplatedOptionProperty<int>*>(property))
    //    return new OptionPropertyWidgetQt<int>(static_cast<TemplatedOptionProperty<int>*>(property));
    //if (dynamic_cast<TextEditorProperty*>(property))
    //    return new TextEditorWidgetQt(static_cast<TextEditorProperty*>(property));
    //if (dynamic_cast<StringOptionProperty*>(property))
    //return new OptionPropertyWidgetQt<std::string>(static_cast<StringOptionProperty*>(property));


    if (dynamic_cast<BoolProperty*>(property))
        return new BoolPropertyWidgetQt(static_cast<BoolProperty*>(property));
    if (dynamic_cast<ButtonProperty*>(property))
        return new ButtonPropertyWidgetQt(static_cast<ButtonProperty*>(property));
    if (dynamic_cast<CompositeProperty*>(property))
        return new CompositePropertyWidgetQt(static_cast<CompositeProperty*>(property));
    if (dynamic_cast<FileProperty*>(property))
        return new FilePropertyWidgetQt(static_cast<FileProperty*>(property));
	if (dynamic_cast<FloatMat2Property*>(property))
		return new FloatMat2PropertyWidgetQt(static_cast<FloatMat2Property*>(property));
    if (dynamic_cast<FloatMat3Property*>(property))
        return new FloatMat3PropertyWidgetQt(static_cast<FloatMat3Property*>(property));
    if (dynamic_cast<FloatMat4Property*>(property))
        return new FloatMat4PropertyWidgetQt(static_cast<FloatMat4Property*>(property));
    if (dynamic_cast<FloatProperty*>(property))
        return new FloatPropertyWidgetQt(static_cast<FloatProperty*>(property));
    if (dynamic_cast<FloatVec2Property*>(property))
        return new FloatVec2PropertyWidgetQt(static_cast<FloatVec2Property*>(property));
    if (dynamic_cast<FloatVec3Property*>(property))
        return new FloatVec3PropertyWidgetQt(static_cast<FloatVec3Property*>(property));
    if (dynamic_cast<FloatVec4Property*>(property))
        return new FloatVec4PropertyWidgetQt(static_cast<FloatVec4Property*>(property));
    if (dynamic_cast<IntVec2Property*>(property))
        return new IntVec2PropertyWidgetQt(static_cast<IntVec2Property*>(property));
    if (dynamic_cast<IntVec3Property*>(property))
        return new IntVec3PropertyWidgetQt(static_cast<IntVec3Property*>(property));
    if (dynamic_cast<IntVec4Property*>(property))
        return new IntVec4PropertyWidgetQt(static_cast<IntVec4Property*>(property));
    if (dynamic_cast<IntProperty*>(property))
        return new IntPropertyWidgetQt(static_cast<IntProperty*>(property));
    if (dynamic_cast<SnapshotProperty*>(property))
        return new SnapshotPropertyWidgetQt(static_cast<SnapshotProperty*>(property));
    if (dynamic_cast<StringProperty*>(property))
        return new StringPropertyWidgetQt(static_cast<StringProperty*>(property));
    if (dynamic_cast<TransferProperty*>(property))
        return new TransferPropertyWidgetQt(static_cast<TransferProperty*>(property));

    LogWarn("No widget for property " + property->getIdentifier() + " found.")
        return 0;
}

} // namespace
