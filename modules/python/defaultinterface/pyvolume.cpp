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

#include "pyutil.h"

#include "../pythoninterface/pyvalueparser.h"

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/processors/processor.h>
#include <modules/opengl/canvasprocessorgl.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/ports/volumeport.h>

namespace inviwo {
    PyObject* py_setVoxel(PyObject* /*self*/, PyObject* args){
        if (PyTuple_Size(args) != 3) {
            std::ostringstream errStr;
            errStr << "setVoxel(vol,(x,y,z),v) takes 3 argument: volume, voxel position and value";
            errStr << " (" << PyTuple_Size(args) << " given)";
            PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
            return 0;
        }

        if (!PyString_Check(PyTuple_GetItem(args, 0))) {
            PyErr_SetString(PyExc_TypeError, "setVoxel(vol,(x,y,z),v). first argument must be a string");
            return 0;
        }


        if (!PyTuple_Check(PyTuple_GetItem(args, 1))) {
            PyErr_SetString(PyExc_TypeError, "setVoxel(vol,(x,y,z),v). second argument must be a tuple");
            return 0;
        }


        if (PyTuple_Size(PyTuple_GetItem(args, 1)) != 3) {
            PyErr_SetString(PyExc_TypeError, (std::string("setVoxel(vol,(x,y,z),v). second argument must be a tuple of size three, got size ") + toString(PyTuple_Size(PyTuple_GetItem(args, 1)))).c_str());
            return 0;
        }


        if (!PyInt_Check(PyTuple_GetItem(PyTuple_GetItem(args, 1),0))) {
            PyErr_SetString(PyExc_TypeError, ("setVoxel(vol,(x,y,z),v). second argument must be a tuple of only integers"));
            return 0;
        }

        if (!PyInt_Check(PyTuple_GetItem(PyTuple_GetItem(args, 1),1))) {
            PyErr_SetString(PyExc_TypeError, ("setVoxel(vol,(x,y,z),v). second argument must be a tuple of only integers"));
            return 0;
        }

        if (!PyInt_Check(PyTuple_GetItem(PyTuple_GetItem(args, 1),2))) {
            PyErr_SetString(PyExc_TypeError, ("setVoxel(vol,(x,y,z),v). second argument must be a tuple of only integers"));
            return 0;
        }

        if(!PyFloat_Check(PyTuple_GetItem(args, 2))){
            PyErr_SetString(PyExc_TypeError, "setVoxel(vol,(x,y,z),v). second argument must be a float");
            return 0;
        }
        PyValueParser parser;

        std::string volume = parser.parse<std::string>(PyTuple_GetItem(args, 0));
        uvec3 voxel = parser.parse<uvec3>(PyTuple_GetItem(args, 1));
        float value = parser.parse<float>(PyTuple_GetItem(args, 2));

        Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(volume);
        if(!processor){
            PyErr_SetString(PyExc_TypeError, ("setVoxel(vol,(x,y,z),v). No processor with id " + volume).c_str());
            return 0;
        }

        std::vector<Outport*> ports = processor->getOutports();
        VolumeOutport* volumePort = 0;
        for(size_t i = 0;i<ports.size() && volumePort==0;i++){
            volumePort = dynamic_cast<VolumeOutport*>(ports[i]);
        }

        if(!volumePort){
            PyErr_SetString(PyExc_TypeError, ("setVoxel(vol,(x,y,z),v). No volume outport on processor " + volume).c_str());
            return 0;
        }
        VolumeRAM* vol = volumePort->getData()->getEditableRepresentation<VolumeRAM>();
        //VolumeRAM* vol = volumePort->getData()->getEditableRepresentation<VolumeRAMPrecision<float>>();
        vol->setValueFromSingleFloat(voxel,value*255);
        Py_RETURN_NONE;
    }

    PyObject* py_getVolumeDimension(PyObject* /*self*/, PyObject* args){
        if (PyTuple_Size(args) != 1) {
            std::ostringstream errStr;
            errStr << "getVolumeDimension(vol) takes 1 argument: volume";
            errStr << " (" << PyTuple_Size(args) << " given)";
            PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
            return 0;
        }

        if (!PyString_Check(PyTuple_GetItem(args, 0))) {
            PyErr_SetString(PyExc_TypeError, "getVolumeDimension(vol). Argument must be a string");
            return 0;
        }

        PyValueParser parser;
        std::string volume = parser.parse<std::string>(PyTuple_GetItem(args, 0));

        Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(volume);
        if(!processor){
            PyErr_SetString(PyExc_TypeError, ("setVoxel(vol,(x,y,z),v). No processor with id " + volume).c_str());
            return 0;
        }

        std::vector<Outport*> ports = processor->getOutports();
        VolumeOutport* volumePort = 0;
        for(size_t i = 0;i<ports.size() && volumePort==0;i++){
            volumePort = dynamic_cast<VolumeOutport*>(ports[i]);
        }

        if(!volumePort){
            PyErr_SetString(PyExc_TypeError, ("setVoxel(vol,(x,y,z),v). No volume outport on processor " + volume).c_str());
            return 0;
        }

        return parser.toPyObject(volumePort->getData()->getDimension());
    }



    PyObject* py_saveTransferFunction(PyObject* /*self*/, PyObject* args){
        if (PyTuple_Size(args) != 3) {
            std::ostringstream errStr;
            errStr << "saveTransferFunction() takes exactly 3 arguments: processor name, property id and filename";
            errStr << " (" << PyTuple_Size(args) << " given)";
            PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
            return 0;
        }

        // check parameter 1 and 2, if they are strings
        if (!PyString_Check(PyTuple_GetItem(args, 0)) || !PyString_Check(PyTuple_GetItem(args, 1) || !PyString_Check(PyTuple_GetItem(args, 2)))) {
            PyErr_SetString(PyExc_TypeError, "saveTransferFunction() arguments must be strings");
            return 0;
        }

        std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
        std::string propertyID = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));
        std::string filename = std::string(PyString_AsString(PyTuple_GetItem(args, 2)));


        Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);
        if(!processor){
            std::string msg = std::string("saveTransferFunction() no processor with name: ") + processorName;
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }

        Property *theProperty = processor->getPropertyByIdentifier(propertyID);
        if(!theProperty){
            std::string msg = std::string("saveTransferFunction() no property with id: ") + propertyID;
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }

        TransferFunctionProperty* tf = dynamic_cast<TransferFunctionProperty*>(theProperty);
        if(!tf){
            std::string msg = std::string("saveTransferFunction() no transfer function property with id: ") + propertyID + ", ("+propertyID  +" is of type "+ theProperty->getClassName() +  ")";
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }

/*
        IvwSerializer serializer(filename);
        serializer.serialize("transferfunction",tf);
        serializer.writeFile();*/

        Py_RETURN_NONE;
    }

    PyObject* py_loadTransferFunction(PyObject* /*self*/, PyObject* args){
        if (PyTuple_Size(args) != 3) {
            std::ostringstream errStr;
            errStr << "loadTransferFunction() takes exactly 3 arguments: processor name, property id and filename";
            errStr << " (" << PyTuple_Size(args) << " given)";
            PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
            return 0;
        }

        // check parameter 1 and 2, if they are strings
        if (!PyString_Check(PyTuple_GetItem(args, 0)) || !PyString_Check(PyTuple_GetItem(args, 1) || !PyString_Check(PyTuple_GetItem(args, 2)))) {
            PyErr_SetString(PyExc_TypeError, "loadTransferFunction() arguments must be strings");
            return 0;
        }

        std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
        std::string propertyID = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));
        std::string filename = std::string(PyString_AsString(PyTuple_GetItem(args, 2)));


        Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);
        if(!processor){
            std::string msg = std::string("loadTransferFunction() no processor with name: ") + processorName;
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }

        Property *theProperty = processor->getPropertyByIdentifier(propertyID);
        if(!theProperty){
            std::string msg = std::string("loadTransferFunction() no property with id: ") + propertyID;
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }

        TransferFunctionProperty* tf = dynamic_cast<TransferFunctionProperty*>(theProperty);
        if(!tf){
            std::string msg = std::string("loadTransferFunction() no transfer function property with id: ") + propertyID + ", ("+propertyID  +" is of type "+ theProperty->getClassName() +  ")";
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }

/*
        IvwDeserializer deserializer(filename);
        deserializer.deserialize("transferfunction",tf);*/

        Py_RETURN_NONE;
    }




}



