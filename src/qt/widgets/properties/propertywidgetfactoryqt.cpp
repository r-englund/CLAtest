/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
#include <inviwo/core/properties/propertysemantics.h>

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
#include <inviwo/qt/widgets/properties/collapsivegroupboxwidgetqt.h>
#include <inviwo/qt/widgets/properties/colorpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/compositepropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/directorypropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/filepropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatminmaxpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatmat2propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatmat3propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatmat4propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatvec2propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatvec3propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/floatvec4propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/intminmaxpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/intpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/intvec2propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/intvec3propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/intvec4propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/lightpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/stringpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/texteditorwidgetqt.h>
#include <inviwo/qt/widgets/properties/imageeditorwidgetqt.h>
#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>

#include <inviwo/qt/widgets/properties/syntaxhighlighter.h>


namespace inviwo {

PropertyWidgetFactoryQt::PropertyWidgetFactoryQt() {}

PropertyWidgetQt* PropertyWidgetFactoryQt::create(Property* property) {
    if (dynamic_cast<BoolProperty*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new BoolPropertyWidgetQt(static_cast<BoolProperty*>(property));
    }
    else if (dynamic_cast<ButtonProperty*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new ButtonPropertyWidgetQt(static_cast<ButtonProperty*>(property));
    }
    else if (dynamic_cast<CompositeProperty*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new CompositePropertyWidgetQt(static_cast<CompositeProperty*>(property));
    }
    else if (dynamic_cast<DirectoryProperty*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new DirectoryPropertyWidgetQt(static_cast<DirectoryProperty*>(property));
    }
    else if (dynamic_cast<EventProperty*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new EventPropertyWidgetQt(static_cast<EventProperty*>(property));
    }
    else if (dynamic_cast<FileProperty*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default ||
                  property->getSemantics()==PropertySemantics::Editor  ||
                  property->getSemantics()==PropertySemantics::Shader,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");                  
        if (property->getSemantics()==PropertySemantics::Editor) {
            if (dynamic_cast<ImageEditorProperty*>(property)) 
                return new ImageEditorWidgetQt(static_cast<FileProperty*>(property));
            else 
                return new TextEditorWidgetQt(static_cast<FileProperty*>(property));
        } else if (property->getSemantics()==PropertySemantics::Shader) {
            TextEditorWidgetQt* editor = new TextEditorWidgetQt(property);
            editor->getSyntaxHighligther()->setSyntax<GLSL>();
            return editor;
        } else
            return new FilePropertyWidgetQt(static_cast<FileProperty*>(property));
    }
    else if (dynamic_cast<FloatMat2Property*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new FloatMat2PropertyWidgetQt(static_cast<FloatMat2Property*>(property));
    }
    else if (dynamic_cast<FloatMat3Property*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new FloatMat3PropertyWidgetQt(static_cast<FloatMat3Property*>(property));
    }
    else if (dynamic_cast<FloatMat4Property*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new FloatMat4PropertyWidgetQt(static_cast<FloatMat4Property*>(property));
    }
    else if (dynamic_cast<FloatProperty*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new FloatPropertyWidgetQt(static_cast<FloatProperty*>(property));
    }
    else if (dynamic_cast<FloatMinMaxProperty*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new FloatMinMaxPropertyWidgetQt(static_cast<FloatMinMaxProperty*>(property));
    }
    else if (dynamic_cast<FloatVec2Property*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new FloatVec2PropertyWidgetQt(static_cast<FloatVec2Property*>(property));
    }
    else if (dynamic_cast<FloatVec3Property*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default ||
                  property->getSemantics()==PropertySemantics::Color,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        if (property->getSemantics()==PropertySemantics::Color)
            return new ColorPropertyWidgetQt(static_cast<FloatVec3Property*>(property));
        else
            return new FloatVec3PropertyWidgetQt(static_cast<FloatVec3Property*>(property));
    }
    else if (dynamic_cast<FloatVec4Property*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default ||
                  property->getSemantics()==PropertySemantics::Color,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        if (property->getSemantics()==PropertySemantics::Color)
            return new ColorPropertyWidgetQt(static_cast<FloatVec4Property*>(property));
        else
            return new FloatVec4PropertyWidgetQt(static_cast<FloatVec4Property*>(property));
    }
    else if (dynamic_cast<IntMinMaxProperty*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new IntMinMaxPropertyWidgetQt(static_cast<IntMinMaxProperty*>(property));
    }
    else if (dynamic_cast<IntVec2Property*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new IntVec2PropertyWidgetQt(static_cast<IntVec2Property*>(property));
    }
    else if (dynamic_cast<IntVec3Property*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default ||
                  property->getSemantics()==PropertySemantics::LightPosition,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        if (property->getSemantics()==PropertySemantics::LightPosition)
            return new LightPropertyWidgetQt(static_cast<IntVec3Property*>(property));
        else
            return new IntVec3PropertyWidgetQt(static_cast<IntVec3Property*>(property));
    }
    else if (dynamic_cast<IntVec4Property*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default ||
                  property->getSemantics()==PropertySemantics::Color,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        if (property->getSemantics()==PropertySemantics::Color)
            return new ColorPropertyWidgetQt(static_cast<IntVec4Property*>(property));
        else
            return new IntVec4PropertyWidgetQt(static_cast<IntVec4Property*>(property));
    }
    else if (dynamic_cast<IntProperty*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new IntPropertyWidgetQt(static_cast<IntProperty*>(property));
    }
    else if (dynamic_cast<BaseOptionProperty*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new OptionPropertyWidgetQt(static_cast<BaseOptionProperty*>(property));
    }
    else if (dynamic_cast<StringProperty*>(property)) {
        switch (property->getSemantics())
        {
        case PropertySemantics::Default:
            return new StringPropertyWidgetQt(static_cast<StringProperty*>(property));
        case PropertySemantics::Editor:
            return new TextEditorWidgetQt(property);
            break;
        case PropertySemantics::Shader:
         {
             TextEditorWidgetQt* editor = new TextEditorWidgetQt(property);
             editor->getSyntaxHighligther()->setSyntax<GLSL>();
             return editor;
        }
        default:
            ivwAssert(false,
                "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
            break;
        }
        
        return new StringPropertyWidgetQt(static_cast<StringProperty*>(property));
    }
    else if (dynamic_cast<TransferFunctionProperty*>(property)) {
        ivwAssert(property->getSemantics()==PropertySemantics::Default,
                  "PropertySemantics::"+getPropertySemanticID(property->getSemantics())+" unsupported for "+property->getClassName()+".");
        return new TransferFunctionPropertyWidgetQt(static_cast<TransferFunctionProperty*>(property));
    }
    else {
        LogWarn("No widget for property " + property->getIdentifier() + " found.")
        return 0;
    }
}

} // namespace
