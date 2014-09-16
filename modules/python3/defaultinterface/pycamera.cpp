/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file author: Rickard Englund
 *
 *********************************************************************************/

#include <modules/python3/pythonincluder.h>

#include "pycamera.h"
#include <math.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <modules/python3/pythoninterface/pyvalueparser.h>


namespace inviwo {

template <typename T>
T arctan(T x, T y) {
    if (x == 0) {
        return static_cast<T>(M_PI_2);
    } else if (x < 0 && y > 0) {
        return static_cast<T>(std::atan(static_cast<double>(y / x)) + M_PI);
    } else if (x < 0 && y < 0) {
        return static_cast<T>(std::atan(static_cast<double>(y / x)) - M_PI);
    } else {
        return static_cast<T>(std::atan(static_cast<double>(y / x)));
    }
};

PyObject* py_setCameraFocus(PyObject* /*self*/, PyObject* args) {
    static PySetCameraFocusMethod p;

    if (!p.testParams(args))
        return 0;

    std::string processorName = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1)));
    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);

    if (!processor) {
        std::string msg = std::string("setCameraFocus() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property* theProperty = processor->getPropertyByIdentifier(propertyID);

    if (!theProperty) {
        std::string msg = std::string("setCameraFocus() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    const std::string className = theProperty->getClassIdentifier();

    if (className == "CameraProperty") {
        vec3 focus;
        char* dummy1,*dummy2;
        int d1,d2;

        if (!PyArg_ParseTuple(args,"s#s#(fff)", &dummy1,&d1,&dummy2,&d2,
                              &focus.x,&focus.y,&focus.z
                             )) {
            std::string msg = std::string("setCameraFocus() Failed to parse values for camera, needs to be on the format: (posX,posY,posZ) ") +
                              propertyID;
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }

        CameraProperty* cam = static_cast<CameraProperty*>(theProperty);
        cam->setLookTo(focus);
        Py_RETURN_NONE;
    } else {
        std::string msg = std::string("setCameraFocus() not a cmera property: ") + className;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }
}

PyObject* py_setCameraUp(PyObject* /*self*/, PyObject* args) {
    static PySetCameraUpMethod p;

    if (!p.testParams(args))
        return 0;

    std::string processorName = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1)));
    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);

    if (!processor) {
        std::string msg = std::string("setCameraUp() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property* theProperty = processor->getPropertyByIdentifier(propertyID);

    if (!theProperty) {
        std::string msg = std::string("setCameraUp() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    const std::string className = theProperty->getClassIdentifier();

    if (className == "CameraProperty") {
        vec3 up;
        char* dummy1,*dummy2;
        int d1,d2;

        if (!PyArg_ParseTuple(args,"s#s#(fff)", &dummy1,&d1,&dummy2,&d2,
                              &up.x,&up.y,&up.z
                             )) {
            std::string msg = std::string("setCameraUp() Failed to parse values for camera, needs to be on the format: (dirX,dirY,dirZ) ") + propertyID;
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }

        CameraProperty* cam = static_cast<CameraProperty*>(theProperty);
        cam->setLookUp(up);
        Py_RETURN_NONE;
    } else {
        std::string msg = std::string("setCameraUp() not a cmera property: ") + className;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }
}



PyObject* py_setCameraPos(PyObject* /*self*/, PyObject* args) {
    static PySetCameraPosMethod p;

    if (!p.testParams(args))
        return 0;

    std::string processorName = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1)));
    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);

    if (!processor) {
        std::string msg = std::string("setCameraPosition() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property* theProperty = processor->getPropertyByIdentifier(propertyID);

    if (!theProperty) {
        std::string msg = std::string("setCameraPosition() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    const std::string className = theProperty->getClassIdentifier();

    if (className == "CameraProperty") {
        vec3 from;
        char* dummy1,*dummy2;
        int d1,d2;

        if (!PyArg_ParseTuple(args,"s#s#(fff)", &dummy1,&d1,&dummy2,&d2,
                              &from.x,&from.y,&from.z
                             )) {
            std::string msg = std::string("setPropertyValue() Failed to parse values for camera, needs to be on the format: (posX,posY,posZ) ") +
                              propertyID;
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }

        CameraProperty* cam = static_cast<CameraProperty*>(theProperty);

        /*vec3 polFrom = vec3(std::sqrt(static_cast<float>(from[0] * from[0] + from[1] * from[1] + from[2] * from[2])),
            arctan<float>(from[2], std::sqrt(static_cast<float>(from[0] * from[0] + from[1] * from[1]))),
            arctan<float>(from[0], from[1]));*/

       vec3 polFrom = vec3(from[0] * std::sin(static_cast<double>(from[1])) * std::cos(static_cast<double>(from[2])),
            from[0] * std::sin(static_cast<double>(from[1])) * std::sin(static_cast<double>(from[2])),
            from[0] * std::cos(static_cast<double>(from[1])));

       polFrom.y = cam->getLookFrom().y;

        cam->setLookFrom(polFrom);
        Py_RETURN_NONE;
    } else {
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