#include <inviwo/qt/widgets/qtwidgetmodule.h>

#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/properties/imageeditorproperty.h>
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
#include <inviwo/qt/widgets/properties/collapsiblegroupboxwidgetqt.h>
#include <inviwo/qt/widgets/properties/colorpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/compositepropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/directorypropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/filepropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatmat2propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatmat3propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatmat4propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatminmaxpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatvec2propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatvec3propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatvec4propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/imageeditorwidgetqt.h>
#include <inviwo/qt/widgets/properties/intminmaxpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/intpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/intvec2propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/intvec3propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/intvec4propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/lightpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/stringpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/texteditorwidgetqt.h>
#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>

namespace inviwo {

QtWidgetModule::QtWidgetModule() : InviwoModule() {
    setIdentifier("QtWidget");

    // What really happens
    //registerPropertyWidgetObject(
    //    new PropertyWidgetFactoryObjectTemplate<FloatPropertyWidgetQt, FloatProperty>("FloatProperty", "Default")
    //    );

    registerPropertyWidget(BoolPropertyWidgetQt, BoolProperty, "Default");
    registerPropertyWidget(ButtonPropertyWidgetQt, ButtonProperty, "Default");
    registerPropertyWidget(CameraPropertyWidgetQt, CameraProperty, "Default");
    registerPropertyWidget(ColorPropertyWidgetQt, IntVec3Property, "Color");
    registerPropertyWidget(ColorPropertyWidgetQt, IntVec4Property, "Color");
    registerPropertyWidget(ColorPropertyWidgetQt, FloatVec3Property, "Color");
    registerPropertyWidget(ColorPropertyWidgetQt, FloatVec4Property, "Color");
    registerPropertyWidget(CompositePropertyWidgetQt, CompositeProperty, "Default");
    registerPropertyWidget(DirectoryPropertyWidgetQt, DirectoryProperty, "Default");
    registerPropertyWidget(EventPropertyWidgetQt, EventProperty, "Default");
    registerPropertyWidget(FilePropertyWidgetQt, FileProperty, "Default");  
    registerPropertyWidget(FloatMat2PropertyWidgetQt, FloatMat2Property, "Default");
    registerPropertyWidget(FloatMat3PropertyWidgetQt, FloatMat3Property, "Default");
    registerPropertyWidget(FloatMat4PropertyWidgetQt, FloatMat4Property, "Default");
    registerPropertyWidget(FloatMinMaxPropertyWidgetQt, FloatMinMaxProperty, "Default");
    registerPropertyWidget(FloatPropertyWidgetQt, FloatProperty, "Default");
    registerPropertyWidget(FloatVec2PropertyWidgetQt, FloatVec2Property, "Default");
    registerPropertyWidget(FloatVec3PropertyWidgetQt, FloatVec3Property, "Default");
    registerPropertyWidget(FloatVec4PropertyWidgetQt, FloatVec4Property, "Default");
    registerPropertyWidget(ImageEditorWidgetQt, FileProperty, "ImageEditor");    // TODO: fix "imageEditor..."
    registerPropertyWidget(IntMinMaxPropertyWidgetQt, IntMinMaxProperty, "Default");
    registerPropertyWidget(IntPropertyWidgetQt, IntProperty, "Default");
    registerPropertyWidget(IntVec2PropertyWidgetQt, IntVec2Property, "Default");
    registerPropertyWidget(IntVec3PropertyWidgetQt, IntVec3Property, "Default");
    registerPropertyWidget(IntVec4PropertyWidgetQt, IntVec4Property, "Default");
    registerPropertyWidget(LightPropertyWidgetQt, FloatVec3Property, "LightPosition");     
    registerPropertyWidget(OptionPropertyWidgetQt, BaseOptionProperty, "Default");
    registerPropertyWidget(StringPropertyWidgetQt, StringProperty, "Default");
    registerPropertyWidget(TextEditorWidgetQt, FileProperty, "TextEditor"); // TODO: fix "imageEditor..."
    registerPropertyWidget(TextEditorWidgetQt, FileProperty, "Shader");
    registerPropertyWidget(TextEditorWidgetQt, StringProperty, "Editor");
    registerPropertyWidget(TextEditorWidgetQt, StringProperty, "Shader");
    registerPropertyWidget(TransferFunctionPropertyWidgetQt, TransferFunctionProperty, "Default");

}

} // namespace

