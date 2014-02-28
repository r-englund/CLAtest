/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/qtwidgetmodule.h>

#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/properties/imageeditorproperty.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/stringproperty.h>
#include <inviwo/core/properties/transferfunctionproperty.h>

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
#include <inviwo/qt/widgets/properties/imageeditorwidgetqt.h>
#include <inviwo/qt/widgets/properties/intminmaxpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/lightpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/stringpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/sphericalfloatvec3propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/texteditorwidgetqt.h>
#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>

#include <inviwo/core/io/rawvolumereader.h>
#include <inviwo/qt/widgets/rawdatareaderdialogqt.h>

namespace inviwo {

QtWidgetModule::QtWidgetModule() : InviwoModule() {
    setIdentifier("QtWidget");
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
    registerPropertyWidget(DoublePropertyWidgetQt, DoubleProperty, "Default");
    registerPropertyWidget(DoubleVec2PropertyWidgetQt, DoubleVec2Property, "Default");
    registerPropertyWidget(DoubleVec3PropertyWidgetQt, DoubleVec3Property, "Default");
    registerPropertyWidget(DoubleVec4PropertyWidgetQt, DoubleVec4Property, "Default");
    registerPropertyWidget(ImageEditorWidgetQt, FileProperty, "ImageEditor");
    registerPropertyWidget(ImageEditorWidgetQt, ImageEditorProperty, "Default");
    registerPropertyWidget(ImageEditorWidgetQt, ImageEditorProperty, "ImageEditor");
    registerPropertyWidget(IntMinMaxPropertyWidgetQt, IntMinMaxProperty, "Default");
    registerPropertyWidget(IntPropertyWidgetQt, IntProperty, "Default");
    registerPropertyWidget(IntVec2PropertyWidgetQt, IntVec2Property, "Default");
    registerPropertyWidget(IntVec3PropertyWidgetQt, IntVec3Property, "Default");
    registerPropertyWidget(IntVec4PropertyWidgetQt, IntVec4Property, "Default");
    registerPropertyWidget(LightPropertyWidgetQt, FloatVec3Property, "LightPosition");
    registerPropertyWidget(OptionPropertyWidgetQt, BaseOptionProperty, "Default");
    registerPropertyWidget(OptionPropertyWidgetQt, OptionPropertyInt, "Default");
    registerPropertyWidget(OptionPropertyWidgetQt, OptionPropertyFloat, "Default");
    registerPropertyWidget(OptionPropertyWidgetQt, OptionPropertyDouble, "Default");
    registerPropertyWidget(OptionPropertyWidgetQt, OptionPropertyString, "Default");
//    registerPropertyWidget(SphericalFloatVec3PropertyWidgetQt, FloatVec3Property, "Spherical");
    registerPropertyWidget(StringPropertyWidgetQt, StringProperty, "Default");
    registerPropertyWidget(StringPropertyWidgetQt, StringProperty, "Password");
    registerPropertyWidget(TextEditorWidgetQt, FileProperty, "TextEditor"); // TODO: fix "imageEditor..."
    registerPropertyWidget(TextEditorWidgetQt, FileProperty, "ShaderEditor");
    registerPropertyWidget(TextEditorWidgetQt, StringProperty, "Editor");
    registerPropertyWidget(TextEditorWidgetQt, StringProperty, "ShaderEditor");
    registerPropertyWidget(TransferFunctionPropertyWidgetQt, TransferFunctionProperty, "Default");
    registerDialog("RawVolumeReader", RawDataReaderDialogQt);
}

} // namespace

