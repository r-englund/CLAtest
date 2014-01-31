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

#include "pycamera.h"

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/cameraproperty.h>
#include "../pythoninterface/pyvalueparser.h"


namespace inviwo {

PyObject* py_setCameraFocus(PyObject* /*self*/, PyObject* args){
    static PySetCameraFocusMethod p;
    if(!p.testParams(args))
        return 0;

    std::string processorName = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1)));
    PyObject* parameter = PyTuple_GetItem(args, 2);

    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);
    if(!processor){
        std::string msg = std::string("setCameraFocus() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("setCameraFocus() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    const std::string className = theProperty->getClassName();
    if(className == "CameraProperty"){
        vec3 focus;
        char *dummy1,*dummy2;
        int d1,d2;
        if(!PyArg_ParseTuple(args,"s#s#(fff)", &dummy1,&d1,&dummy2,&d2,
            &focus.x,&focus.y,&focus.z
            )){
                std::string msg = std::string("setCameraFocus() Failed to parse values for camera, needs to be on the format: (posX,posY,posZ) ") + propertyID;
                PyErr_SetString(PyExc_TypeError, msg.c_str());
                return 0;
        }

        CameraProperty* cam = static_cast<CameraProperty*>(theProperty);
        cam->setLookTo(focus);
        Py_RETURN_NONE;
    }else{
        std::string msg = std::string("setCameraFocus() not a cmera property: ") + className;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }
}

PyObject* py_setCameraUp(PyObject* /*self*/, PyObject* args){
    static PySetCameraUpMethod p;
    if(!p.testParams(args))
        return 0;


    std::string processorName = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1)));
    PyObject* parameter = PyTuple_GetItem(args, 2);
    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);
    if(!processor){
        std::string msg = std::string("setCameraUp() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("setCameraUp() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    const std::string className = theProperty->getClassName();
    if(className == "CameraProperty"){
        vec3 up;
        char *dummy1,*dummy2;
        int d1,d2;
        if(!PyArg_ParseTuple(args,"s#s#(fff)", &dummy1,&d1,&dummy2,&d2,
            &up.x,&up.y,&up.z
            )){
                std::string msg = std::string("setCameraUp() Failed to parse values for camera, needs to be on the format: (dirX,dirY,dirZ) ") + propertyID;
                PyErr_SetString(PyExc_TypeError, msg.c_str());
                return 0;
        }

        CameraProperty* cam = static_cast<CameraProperty*>(theProperty);
        cam->setLookUp(up);
        Py_RETURN_NONE;
    }else{
        std::string msg = std::string("setCameraUp() not a cmera property: ") + className;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }
}



PyObject* py_setCameraPos(PyObject* /*self*/, PyObject* args){
    static PySetCameraPosMethod p;
    if(!p.testParams(args))
        return 0;


    std::string processorName = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1)));
    PyObject* parameter = PyTuple_GetItem(args, 2);

    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);
    if(!processor){
        std::string msg = std::string("setCameraPosition() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("setCameraPosition() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    const std::string className = theProperty->getClassName();
    if(className == "CameraProperty"){
        vec3 from;
        char *dummy1,*dummy2;
        int d1,d2;
        if(!PyArg_ParseTuple(args,"s#s#(fff)", &dummy1,&d1,&dummy2,&d2,
            &from.x,&from.y,&from.z
            )){
                std::string msg = std::string("setPropertyValue() Failed to parse values for camera, needs to be on the format: (posX,posY,posZ) ") + propertyID;
                PyErr_SetString(PyExc_TypeError, msg.c_str());
                return 0;
        }

        CameraProperty* cam = static_cast<CameraProperty*>(theProperty);
        cam->setLookFrom(from);
        Py_RETURN_NONE;
    }else{
        std::string msg = std::string("setCameraPosition() not a cmera property: ") + className;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }
}


PySetCameraFocusMethod::PySetCameraFocusMethod()
    : processor_("processor")
    , property_("property")
    , focusPoint_("focusPoint")
{
    addParam(&processor_);
    addParam(&property_);
    addParam(&focusPoint_);
}

PySetCameraUpMethod::PySetCameraUpMethod()
    : processor_("processor")
    , property_("property")
    , upVector_("upVector")
{
    addParam(&processor_);
    addParam(&property_);
    addParam(&upVector_);
}

PySetCameraPosMethod::PySetCameraPosMethod()
    : processor_("processor")
    , property_("property")
    , position_("position")
{
    addParam(&processor_);
    addParam(&property_);
    addParam(&position_);
}

}