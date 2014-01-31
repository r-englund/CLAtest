/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Rickard Englund
 *
 **********************************************************************/

#include "pyproperties.h"


#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/processors/processor.h>

#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/directoryproperty.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/properties/matrixproperties.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <inviwo/core/properties/vectorproperties.h>
#include <inviwo/core/properties/stringproperty.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include "../pythoninterface/pyvalueparser.h"

namespace inviwo {

PyObject* py_setPropertyValue(PyObject* self, PyObject* args){
    static PySetPropertyValueMethod p;
    if(!p.testParams(args))
        return 0;

    std::string processorName = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1)));
    PyObject* parameter = PyTuple_GetItem(args, 2);

    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);
    if(!processor){
        std::string msg = std::string("setPropertyValue() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("setPropertyValue() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    PyValueParser::setProperty(theProperty,parameter);

    Py_RETURN_NONE;
}





PyObject* py_setPropertyMaxValue(PyObject* /*self*/, PyObject* args){
    static PySetPropertyMaxValueMethod p;
    if(!p.testParams(args))
        return 0;

    std::string processorName = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1)));
    PyObject* parameter = PyTuple_GetItem(args, 2);

    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);
    if(!processor){
        std::string msg = std::string("setPropertyMaxValue() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("setPropertyMaxValue() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    OrdinalProperty<float>* ordinalFloat = dynamic_cast<OrdinalProperty<float>*>(theProperty);
    OrdinalProperty<int>*   ordinalInt   = dynamic_cast<OrdinalProperty<int>  *>(theProperty);
    OrdinalProperty<ivec2>* ordinalIvec2 = dynamic_cast<OrdinalProperty<ivec2>*>(theProperty);
    OrdinalProperty<ivec3>* ordinalIvec3 = dynamic_cast<OrdinalProperty<ivec3>*>(theProperty);
    OrdinalProperty<ivec4>* ordinalIvec4 = dynamic_cast<OrdinalProperty<ivec4>*>(theProperty);
    OrdinalProperty<mat2>*  ordinalMat2  = dynamic_cast<OrdinalProperty<mat2> *>(theProperty);
    OrdinalProperty<mat3>*  ordinalMat3  = dynamic_cast<OrdinalProperty<mat3> *>(theProperty);
    OrdinalProperty<mat4>*  ordinalMat4  = dynamic_cast<OrdinalProperty<mat4> *>(theProperty);
    OrdinalProperty<vec2>*  ordinalVec2  = dynamic_cast<OrdinalProperty<vec2> *>(theProperty);
    OrdinalProperty<vec3>*  ordinalVec3  = dynamic_cast<OrdinalProperty<vec3> *>(theProperty);
    OrdinalProperty<vec4>*  ordinalVec4  = dynamic_cast<OrdinalProperty<vec4> *>(theProperty);

    if(ordinalFloat){
        ordinalFloat->setMaxValue(PyValueParser::parse<float>(args));
    }else if(ordinalInt){
        ordinalInt->setMaxValue(PyValueParser::parse<int>(args));
    }else if(ordinalIvec2){
        ordinalIvec2->setMaxValue(PyValueParser::parse<ivec2>(args));
    }else if(ordinalIvec3){
        ordinalIvec3->setMaxValue(PyValueParser::parse<ivec3>(args));
    }else if(ordinalIvec4){
        ordinalIvec4->setMaxValue(PyValueParser::parse<ivec4>(args));
    }else if(ordinalMat2){
        ordinalMat2->setMaxValue(PyValueParser::parse<mat2>(args));
    }else if(ordinalMat3){
        ordinalMat3->setMaxValue(PyValueParser::parse<mat3>(args));
    }else if(ordinalMat4){
        ordinalMat4->setMaxValue(PyValueParser::parse<mat4>(args));
    }else if(ordinalVec2){
        ordinalVec2->setMaxValue(PyValueParser::parse<vec2>(args));
    }else if(ordinalVec3){
        ordinalVec3->setMaxValue(PyValueParser::parse<vec3>(args));
    }else if(ordinalVec4){
        ordinalVec4->setMaxValue(PyValueParser::parse<vec4>(args));
    }else{
        LogErrorCustom("inviwo_setPropertyMaxValue","Unknown parameter type");
    }

    Py_RETURN_NONE;
}



PyObject* py_setPropertyMinValue(PyObject* /*self*/, PyObject* args){
    static PySetPropertyMinValueMethod p;
    if(!p.testParams(args))
        return 0;

    std::string processorName = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1)));
    PyObject* parameter = PyTuple_GetItem(args, 2);

    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);
    if(!processor){
        std::string msg = std::string("setPropertyMinValue() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("setPropertyMinValue() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    OrdinalProperty<float>* ordinalFloat = dynamic_cast<OrdinalProperty<float>*>(theProperty);
    OrdinalProperty<int>*   ordinalInt   = dynamic_cast<OrdinalProperty<int>  *>(theProperty);
    OrdinalProperty<ivec2>* ordinalIvec2 = dynamic_cast<OrdinalProperty<ivec2>*>(theProperty);
    OrdinalProperty<ivec3>* ordinalIvec3 = dynamic_cast<OrdinalProperty<ivec3>*>(theProperty);
    OrdinalProperty<ivec4>* ordinalIvec4 = dynamic_cast<OrdinalProperty<ivec4>*>(theProperty);
    OrdinalProperty<mat2>*  ordinalMat2  = dynamic_cast<OrdinalProperty<mat2> *>(theProperty);
    OrdinalProperty<mat3>*  ordinalMat3  = dynamic_cast<OrdinalProperty<mat3> *>(theProperty);
    OrdinalProperty<mat4>*  ordinalMat4  = dynamic_cast<OrdinalProperty<mat4> *>(theProperty);
    OrdinalProperty<vec2>*  ordinalVec2  = dynamic_cast<OrdinalProperty<vec2> *>(theProperty);
    OrdinalProperty<vec3>*  ordinalVec3  = dynamic_cast<OrdinalProperty<vec3> *>(theProperty);
    OrdinalProperty<vec4>*  ordinalVec4  = dynamic_cast<OrdinalProperty<vec4> *>(theProperty);

    if(ordinalFloat){
        ordinalFloat->setMinValue(PyValueParser::parse<float>(args));
    }else if(ordinalInt){
        ordinalInt->setMinValue(PyValueParser::parse<int>(args));
    }else if(ordinalIvec2){
        ordinalIvec2->setMinValue(PyValueParser::parse<ivec2>(args));
    }else if(ordinalIvec3){
        ordinalIvec3->setMinValue(PyValueParser::parse<ivec3>(args));
    }else if(ordinalIvec4){
        ordinalIvec4->setMinValue(PyValueParser::parse<ivec4>(args));
    }else if(ordinalMat2){
        ordinalMat2->setMinValue(PyValueParser::parse<mat2>(args));
    }else if(ordinalMat3){
        ordinalMat3->setMinValue(PyValueParser::parse<mat3>(args));
    }else if(ordinalMat4){
        ordinalMat4->setMinValue(PyValueParser::parse<mat4>(args));
    }else if(ordinalVec2){
        ordinalVec2->setMinValue(PyValueParser::parse<vec2>(args));
    }else if(ordinalVec3){
        ordinalVec3->setMinValue(PyValueParser::parse<vec3>(args));
    }else if(ordinalVec4){
        ordinalVec4->setMinValue(PyValueParser::parse<vec4>(args));
    }else{
        LogErrorCustom("inviwo_setPropertyMinValue","Unknown parameter type" );
    }

    Py_RETURN_NONE;
}




PyObject* py_getPropertyValue(PyObject* /*self*/, PyObject* args){
    static PyGetPropertyValueMethod p;
    if(!p.testParams(args))
        return 0;

    std::string processorName = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1)));


    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);
    if(!processor){
        std::string msg = std::string("getPropertyValue() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("getPropertyValue() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    return PyValueParser::getProperty(theProperty);
}

#define CAST_N_GETMAX(PropType,prop,parser) {PropType* casted = dynamic_cast<PropType*>(prop); if(casted) {return PyValueParser::toPyObject(casted->getMaxValue());}}
#define CAST_N_GETMIN(PropType,prop,parser) {PropType* casted = dynamic_cast<PropType*>(prop); if(casted) {return PyValueParser::toPyObject(casted->getMinValue());}}

PyObject* py_getPropertyMaxValue(PyObject* /*self*/, PyObject* args){

    static PyGetPropertyMaxValueMethod p;
    if(!p.testParams(args))
        return 0;

    std::string processorName = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1)));


    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);
    if(!processor){
        std::string msg = std::string("getPropertyMaxValue() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("getPropertyMaxValue() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    CAST_N_GETMAX(FloatProperty,theProperty,parser);
    CAST_N_GETMAX(IntProperty,theProperty,parser);
    CAST_N_GETMAX(IntVec2Property,theProperty,parser);
    CAST_N_GETMAX(IntVec3Property,theProperty,parser);
    CAST_N_GETMAX(IntVec4Property,theProperty,parser);
    CAST_N_GETMAX(FloatMat2Property,theProperty,parser);
    CAST_N_GETMAX(FloatMat3Property,theProperty,parser);
    CAST_N_GETMAX(FloatMat4Property,theProperty,parser);
    CAST_N_GETMAX(FloatVec2Property,theProperty,parser);
    CAST_N_GETMAX(FloatVec3Property,theProperty,parser);
    CAST_N_GETMAX(FloatVec4Property,theProperty,parser);

    LogErrorCustom("inviwo_getPropertyMaxValue","Unknown parameter type");

    Py_RETURN_NONE;
}



PyObject* py_getPropertyMinValue(PyObject* /*self*/, PyObject* args){
    static PyGetPropertyMinValueMethod p;
    if(!p.testParams(args))
        return 0;

    std::string processorName = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1)));


    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);
    if(!processor){
        std::string msg = std::string("getPropertyMinValue() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("getPropertyMinValue() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    CAST_N_GETMIN(FloatProperty,theProperty,parser);
    CAST_N_GETMIN(IntProperty,theProperty,parser);
    CAST_N_GETMIN(IntVec2Property,theProperty,parser);
    CAST_N_GETMIN(IntVec3Property,theProperty,parser);
    CAST_N_GETMIN(IntVec4Property,theProperty,parser);
    CAST_N_GETMIN(FloatMat2Property,theProperty,parser);
    CAST_N_GETMIN(FloatMat3Property,theProperty,parser);
    CAST_N_GETMIN(FloatMat4Property,theProperty,parser);
    CAST_N_GETMIN(FloatVec2Property,theProperty,parser);
    CAST_N_GETMIN(FloatVec3Property,theProperty,parser);
    CAST_N_GETMIN(FloatVec4Property,theProperty,parser);

    LogErrorCustom("inviwo_getPropertyMaxValue","Unknown parameter type");

    Py_RETURN_NONE;
}



PyObject* py_clickButton(PyObject* /*self*/, PyObject* args){
    static PyClickButtonMethod p;
    if(!p.testParams(args))
        return 0;

    std::string processorName = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1)));


    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);
    if(!processor){
        std::string msg = std::string("clickButton() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("clickButton() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    ButtonProperty* button = dynamic_cast<ButtonProperty*>(theProperty);
    if(!button){
        std::string msg = std::string("clickButton() no button property with id: ") + propertyID + ", ("+propertyID  +" is of type "+ theProperty->getClassName() +  ")";
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    button->propertyModified();

    Py_RETURN_NONE;
}


PySetPropertyValueMethod::PySetPropertyValueMethod()
    : processor_("processor")
    , property_("property")
    , value_("value")
{
    addParam(&processor_);
    addParam(&property_);
    addParam(&value_);
}

PySetPropertyMaxValueMethod::PySetPropertyMaxValueMethod()
    : processor_("processor")
    , property_("property")
    , maxValue_("maxValue")
{
    addParam(&processor_);
    addParam(&property_);
    addParam(&maxValue_);
}

PySetPropertyMinValueMethod::PySetPropertyMinValueMethod()
    : processor_("processor")
    , property_("property")
    , minValue_("minValue")
{
    addParam(&processor_);
    addParam(&property_);
    addParam(&minValue_);
}

PyGetPropertyValueMethod::PyGetPropertyValueMethod()
    : processor_("processor")
    , property_("property")
{
    addParam(&processor_);
    addParam(&property_);
}

PyGetPropertyMaxValueMethod::PyGetPropertyMaxValueMethod()
    : processor_("processor")
    , property_("property")
{
    addParam(&processor_);
    addParam(&property_);
}

PyGetPropertyMinValueMethod::PyGetPropertyMinValueMethod()
    : processor_("processor")
    , property_("property")
{
    addParam(&processor_);
    addParam(&property_);
}

PyClickButtonMethod::PyClickButtonMethod()
    : processor_("processor")
    , property_("property")
{
    addParam(&processor_);
    addParam(&property_);
}




}