#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>

#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/properties/matrixproperties.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <inviwo/core/properties/stringproperty.h>
#include <inviwo/core/properties/texteditorproperty.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <inviwo/core/properties/vectorproperties.h>

#include <inviwo/qt/widgets/properties/boolpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/buttonpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/camerapropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/collapsivegroupboxwidgetqt.h>
#include <inviwo/qt/widgets/properties/colorpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/compositepropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>
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
#include <inviwo/qt/widgets/properties/stringpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/texteditorwidgetqt.h>
#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>


namespace inviwo {

PropertyWidgetFactoryQt::PropertyWidgetFactoryQt() {}

PropertyWidgetQt* PropertyWidgetFactoryQt::create(Property* property) {

    if (property->getSemantics()!=PropertySemantics::Default){
        if (dynamic_cast<FloatVec4Property*>(property)&& property->getSemantics() == PropertySemantics::Color) {
            return new ColorPropertyWidgetQt(static_cast<FloatVec4Property*>(property));
        }
        if (dynamic_cast<FileProperty*>(property)&& property->getSemantics() == PropertySemantics::Editor) {
            return new TextEditorWidgetQt(static_cast<FileProperty*>(property));
        }
        if (dynamic_cast<IntVec4Property*>(property)&& property->getSemantics() == PropertySemantics::Color) {
            return new ColorPropertyWidgetQt(static_cast<IntVec4Property*>(property));
        }
        if (dynamic_cast<StringProperty*>(property)&& property->getSemantics() == PropertySemantics::Editor) {
            return new TextEditorWidgetQt(static_cast<StringProperty*>(property));
        }
    }
    if (dynamic_cast<BoolProperty*>(property))
        return new BoolPropertyWidgetQt(static_cast<BoolProperty*>(property));
    if (dynamic_cast<ButtonProperty*>(property))
        return new ButtonPropertyWidgetQt(static_cast<ButtonProperty*>(property));
    if (dynamic_cast<CompositeProperty*>(property))
        return new CompositePropertyWidgetQt(static_cast<CompositeProperty*>(property));
    if (dynamic_cast<EventProperty*>(property))
        return new EventPropertyWidgetQt(static_cast<EventProperty*>(property));
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
    if (dynamic_cast<OptionPropertyDouble*>(property))
        return new OptionPropertyWidgetQt(static_cast<OptionPropertyDouble*>(property));
    if (dynamic_cast<OptionPropertyFloat*>(property))
        return new OptionPropertyWidgetQt(static_cast<OptionPropertyFloat*>(property));
    if (dynamic_cast<OptionPropertyInt*>(property))
        return new OptionPropertyWidgetQt(static_cast<OptionPropertyInt*>(property));
    if (dynamic_cast<OptionPropertyString*>(property))
        return new OptionPropertyWidgetQt(static_cast<OptionPropertyString*>(property));
    if (dynamic_cast<StringProperty*>(property))
        return new StringPropertyWidgetQt(static_cast<StringProperty*>(property));
    if (dynamic_cast<TransferFunctionProperty*>(property))
        return new TransferFunctionPropertyWidgetQt(static_cast<TransferFunctionProperty*>(property));

    LogWarn("No widget for property " + property->getIdentifier() + " found.")
        return 0;
}

} // namespace
