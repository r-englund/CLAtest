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
 * Contact: Rickard Englund
 *
 *********************************************************************************/

#include "pyvolume.h"

#include <modules/python/pythoninterface/pyvalueparser.h>

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/processors/processor.h>
#include <modules/opengl/canvasprocessorgl.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <inviwo/core/util/filesystem.h>

namespace inviwo {
PyObject* py_setVoxel(PyObject* /*self*/, PyObject* args) {
    static PySetVoxelMethod p;

    if (!p.testParams(args))
        return 0;

    std::string volume = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0));
    uvec3 voxel = PyValueParser::parse<uvec3>(PyTuple_GetItem(args, 1));
    float value = PyValueParser::parse<float>(PyTuple_GetItem(args, 2));
    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByIdentifier(volume);

    if (!processor) {
        PyErr_SetString(PyExc_TypeError, ("setVoxel(vol,(x,y,z),v). No processor with id " + volume).c_str());
        return 0;
    }

    std::vector<Outport*> ports = processor->getOutports();
    VolumeOutport* volumePort = 0;

    for (size_t i = 0; i<ports.size() && volumePort==0; i++) {
        volumePort = dynamic_cast<VolumeOutport*>(ports[i]);
    }

    if (!volumePort) {
        PyErr_SetString(PyExc_TypeError, ("setVoxel(vol,(x,y,z),v). No volume outport on processor " + volume).c_str());
        return 0;
    }

    VolumeRAM* vol = volumePort->getData()->getEditableRepresentation<VolumeRAM>();
    vol->setValueFromSingleDouble(voxel,value*255);
    Py_RETURN_NONE;
}

PyObject* py_getVolumeDimension(PyObject* /*self*/, PyObject* args) {
    static PyGetVolumeDimension p;

    if (!p.testParams(args))
        return 0;

    std::string volume = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0));
    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByIdentifier(volume);

    if (!processor) {
        PyErr_SetString(PyExc_TypeError, ("setVoxel(vol,(x,y,z),v). No processor with id " + volume).c_str());
        return 0;
    }

    std::vector<Outport*> ports = processor->getOutports();
    VolumeOutport* volumePort = 0;

    for (size_t i = 0; i<ports.size() && volumePort==0; i++) {
        volumePort = dynamic_cast<VolumeOutport*>(ports[i]);
    }

    if (!volumePort) {
        PyErr_SetString(PyExc_TypeError, ("setVoxel(vol,(x,y,z),v). No volume outport on processor " + volume).c_str());
        return 0;
    }

    return PyValueParser::toPyObject(volumePort->getData()->getDimension());
}



PyObject* py_saveTransferFunction(PyObject* /*self*/, PyObject* args) {
    static PySaveTransferFunction p;

    if (!p.testParams(args))
        return 0;

    std::string processorName = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0));
    std::string propertyID    = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1));
    std::string filename      = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 2));
    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByIdentifier(processorName);

    if (!processor) {
        std::string msg = std::string("saveTransferFunction() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property* theProperty = processor->getPropertyByIdentifier(propertyID);

    if (!theProperty) {
        std::string msg = std::string("saveTransferFunction() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    TransferFunctionProperty* tf = dynamic_cast<TransferFunctionProperty*>(theProperty);

    if (!tf) {
        std::string msg = std::string("saveTransferFunction() no transfer function property with id: ") + propertyID + ", ("+propertyID
                          +" is of type "+ theProperty->getClassIdentifier() +  ")";
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    //*
    IvwSerializer serializer(filename);
    tf->serialize(serializer);
    serializer.writeFile();
    //*/
    Py_RETURN_NONE;
}


PyObject* py_loadTransferFunction(PyObject* /*self*/, PyObject* args) {
    static PyLoadTransferFunction p;

    if (!p.testParams(args))
        return 0;

    std::string processorName = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0));
    std::string propertyID    = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1));
    std::string filename      = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 2));
    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByIdentifier(processorName);

    if (!processor) {
        std::string msg = std::string("loadTransferFunction() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property* theProperty = processor->getPropertyByIdentifier(propertyID);

    if (!theProperty) {
        std::string msg = std::string("loadTransferFunction() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    TransferFunctionProperty* tf = dynamic_cast<TransferFunctionProperty*>(theProperty);

    if (!tf) {
        std::string msg = std::string("loadTransferFunction() no transfer function property with id: ") + propertyID + ", ("+propertyID
                          +" is of type "+ theProperty->getClassIdentifier() +  ")";
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    if (!filesystem::fileExists(filename)) {
        if (filesystem::fileExists(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_TRANSFERFUNCTIONS,filename))) {
            filename = InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_TRANSFERFUNCTIONS,filename);
        } else {
            std::string msg = "loadTransferFunction() file not found (" + filename + ")";
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }
    }

    //*
    IvwDeserializer deserializer(filename);
    tf->deserialize(deserializer);
    //*/
    Py_RETURN_NONE;
}


PyObject* py_clearTransferfunction(PyObject* /*self*/, PyObject* args) {
    static PyClearTransferfunction p;

    if (!p.testParams(args))
        return 0;

    std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));
    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByIdentifier(processorName);

    if (!processor) {
        std::string msg = std::string("clearTransferfunction() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property* theProperty = processor->getPropertyByIdentifier(propertyID);

    if (!theProperty) {
        std::string msg = std::string("clearTransferfunction() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    TransferFunctionProperty* tf = dynamic_cast<TransferFunctionProperty*>(theProperty);

    if (!tf) {
        std::string msg = std::string("clearTransferfunction() no transfer function property with id: ") + propertyID + ", ("+propertyID
                          +" is of type "+ theProperty->getClassIdentifier() +  ")";
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    tf->get().clearPoints();
    Py_RETURN_NONE;
}


PyObject* py_addPointTransferFunction(PyObject* /*self*/, PyObject* args) {
    static PyAddTransferFunction p;

    if (!p.testParams(args))
        return 0;

    std::string processorName = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0));
    std::string propertyID    = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1));
    vec2 pos = PyValueParser::parse<vec2>(PyTuple_GetItem(args, 2));
    vec3 color = PyValueParser::parse<vec3>(PyTuple_GetItem(args, 3));
    Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByIdentifier(processorName);

    if (!processor) {
        std::string msg = std::string("addPointToTransferFunction() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property* theProperty = processor->getPropertyByIdentifier(propertyID);

    if (!theProperty) {
        std::string msg = std::string("addPointToTransferFunction() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    TransferFunctionProperty* tf = dynamic_cast<TransferFunctionProperty*>(theProperty);

    if (!tf) {
        std::string msg = std::string("addPointToTransferFunction() no transfer function property with id: ") + propertyID + ", ("+propertyID
                          +" is of type "+ theProperty->getClassIdentifier() +  ")";
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    tf->get().addPoint(pos,vec4(color,pos.y));
    tf->setPropertyModified(true);
    Py_RETURN_NONE;
}



PySetVoxelMethod::PySetVoxelMethod()
    : processor_("processor")
    , property_("property")
    , voxelPosition_("voxelPosition")
    , voxelValue_("voxelValue")
{
    addParam(&processor_);
    addParam(&property_);
    addParam(&voxelPosition_);
    addParam(&voxelValue_);
}

PyGetVolumeDimension::PyGetVolumeDimension()
    : processor_("processor")
{
    addParam(&processor_);
}


PySaveTransferFunction::PySaveTransferFunction()
    : processor_("processor")
    , property_("property")
    , filename_("filename")
{
    addParam(&processor_);
    addParam(&property_);
    addParam(&filename_);
}



PyLoadTransferFunction::PyLoadTransferFunction()
    : processor_("processor")
    , property_("property")
    , filename_("filename")
{
    addParam(&processor_);
    addParam(&property_);
    addParam(&filename_);
}


PyClearTransferfunction::PyClearTransferfunction()
    : processor_("processor")
    , property_("property")
{
    addParam(&processor_);
    addParam(&property_);
}




PyAddTransferFunction::PyAddTransferFunction()
    : processor_("processor")
    , property_("property")
    , pos_("position")
    , color_("color")
{
    addParam(&processor_);
    addParam(&property_);
    addParam(&pos_);
    addParam(&color_);
}

}



