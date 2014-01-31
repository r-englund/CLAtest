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

#include "pyvolume.h"

#include "../pythoninterface/pyvalueparser.h"

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/processors/processor.h>
#include <modules/opengl/canvasprocessorgl.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/properties/transferfunctionproperty.h>

namespace inviwo {
    PyObject* py_setVoxel(PyObject* /*self*/, PyObject* args){
        static PySetVoxelMethod p;
        if(!p.testParams(args))
            return 0;

        std::string volume = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0));
        uvec3 voxel = PyValueParser::parse<uvec3>(PyTuple_GetItem(args, 1));
        float value = PyValueParser::parse<float>(PyTuple_GetItem(args, 2));

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
        vol->setValueFromSingleFloat(voxel,value*255);
        Py_RETURN_NONE;
    }

    PyObject* py_getVolumeDimension(PyObject* /*self*/, PyObject* args){
        static PyGetVolumeDimension p;
        if(!p.testParams(args))
            return 0;

        
        std::string volume = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0));

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

        return PyValueParser::toPyObject(volumePort->getData()->getDimension());
    }



    PyObject* py_saveTransferFunction(PyObject* /*self*/, PyObject* args){
        static PySaveTransferFunction p;
        if(!p.testParams(args))
            return 0;

        std::string processorName = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0));
        std::string propertyID    = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1));
        std::string filename      = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 2));


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


        //*
        IvwSerializer serializer(filename);
        tf->serialize(serializer);
        serializer.writeFile();
        //*/

        Py_RETURN_NONE;
    }


    PyObject* py_loadTransferFunction(PyObject* /*self*/, PyObject* args){
        static PyLoadTransferFunction p;
        if(!p.testParams(args))
            return 0;

        std::string processorName = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0));
        std::string propertyID    = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1));
        std::string filename      = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 2));

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

        if(!URLParser::fileExists(filename)){
            if(URLParser::fileExists(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_TRANSFERFUNCTIONS,filename))){
                filename = InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_TRANSFERFUNCTIONS,filename);
            }else{
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


    PyObject* py_clearTransferfunction(PyObject* /*self*/, PyObject* args){
        static PyClearTransferfunction p;
        if(!p.testParams(args))
            return 0;

        std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
        std::string propertyID = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));


        Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);
        if(!processor){
            std::string msg = std::string("clearTransferfunction() no processor with name: ") + processorName;
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }

        Property *theProperty = processor->getPropertyByIdentifier(propertyID);
        if(!theProperty){
            std::string msg = std::string("clearTransferfunction() no property with id: ") + propertyID;
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }

        TransferFunctionProperty* tf = dynamic_cast<TransferFunctionProperty*>(theProperty);
        if(!tf){
            std::string msg = std::string("clearTransferfunction() no transfer function property with id: ") + propertyID + ", ("+propertyID  +" is of type "+ theProperty->getClassName() +  ")";
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }

        tf->get().clearPoints();

        Py_RETURN_NONE;

    }

    
    PyObject* py_addPointTransferFunction(PyObject* /*self*/, PyObject* args){
        static PyAddTransferFunction p;
        if(!p.testParams(args))
            return 0;
        
        std::string processorName = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0));
        std::string propertyID    = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1));
        vec2 pos = PyValueParser::parse<vec2>(PyTuple_GetItem(args, 2));
        vec3 color = PyValueParser::parse<vec3>(PyTuple_GetItem(args, 3));

        Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorName);
        if(!processor){
            std::string msg = std::string("addPointToTransferFunction() no processor with name: ") + processorName;
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }

        Property *theProperty = processor->getPropertyByIdentifier(propertyID);
        if(!theProperty){
            std::string msg = std::string("addPointToTransferFunction() no property with id: ") + propertyID;
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }

        TransferFunctionProperty* tf = dynamic_cast<TransferFunctionProperty*>(theProperty);
        if(!tf){
            std::string msg = std::string("addPointToTransferFunction() no transfer function property with id: ") + propertyID + ", ("+propertyID  +" is of type "+ theProperty->getClassName() +  ")";
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



