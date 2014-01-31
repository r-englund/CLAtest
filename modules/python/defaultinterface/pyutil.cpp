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



namespace inviwo {

PyObject* py_snapshot(PyObject* /*self*/, PyObject* args){
    static PySnapshotMethod p;
    if(!p.testParams(args))
        return 0;
    
    std::string canvasName = "";
    std::string filename = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0)));
    if(PyTuple_Size(args) == 2){
        canvasName = std::string(PyValueParser::parse<std::string>(PyTuple_GetItem(args, 1)));
    }
    CanvasProcessor *canvas = 0;
    if(canvasName.size()!=0){
        canvas = dynamic_cast<CanvasProcessor*>(InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(canvasName));
    }else{
        if(InviwoApplication::getPtr() && InviwoApplication::getPtr()->getProcessorNetwork()){
            std::vector<CanvasProcessor*> canvases = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>();
            if(canvases.size()!=0)
                canvas = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>()[0];
        }
    }

    if(!canvas){
        PyErr_SetString(PyExc_TypeError, "snapshot() no canvas found");
        return 0;
    }

    canvas->saveImageLayer(filename.c_str());
    Py_RETURN_NONE;
}



PyObject* py_snapshotCanvas(PyObject* /*self*/, PyObject* args){
    static PySnapshotCanvasMethod p;
    if(!p.testParams(args))
        return 0;
    
    unsigned int index;
    const char* filename = 0;
    if (!PyArg_ParseTuple(args, "is:canvasSnapshot", &index, &filename))
        return 0;

    std::vector<CanvasProcessor*> canvases = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>();
    if(index>=canvases.size()){
        std::string msg = std::string("snapshotCanvas() index out of range with index: ") + toString(index) + " ,canvases avilable: " + toString(canvases.size());
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }
    canvases[index]->saveImageLayer(filename);
    Py_RETURN_NONE;
}


PyObject* py_getBasePath(PyObject* /*self*/, PyObject* /*args*/){
    return PyValueParser::toPyObject(InviwoApplication::getPtr()->getBasePath());

}

PyObject* py_getDataPath(PyObject* /*self*/, PyObject* /*args*/){
    return PyValueParser::toPyObject(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_DATA));
}

PyObject* py_getWorkspaceSavePath(PyObject* /*self*/, PyObject* /*args*/){
    return PyValueParser::toPyObject(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_WORKSPACES));
}
PyObject* py_getVolumePath(PyObject* /*self*/, PyObject* /*args*/){
    return PyValueParser::toPyObject(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_VOLUMES));
}
PyObject* py_getImagePath(PyObject* /*self*/, PyObject* /*args*/){
    return PyValueParser::toPyObject(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_IMAGES));
}
PyObject* py_getModulePath(PyObject* /*self*/, PyObject* /*args*/){
    return PyValueParser::toPyObject(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES));
}

PyObject* py_getTransferFunctionPath(PyObject* /*self*/, PyObject* /*args*/){
    return PyValueParser::toPyObject(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_TRANSFERFUNCTIONS));
}


PyObject* py_quitInviwo(PyObject* /*self*/, PyObject* /*args*/){
    
    InviwoApplication::getPtr()->closeInviwoApplication();

    Py_RETURN_NONE;
}

PyObject* py_disableEvaluation(PyObject* /*self*/, PyObject* /*args*/){
    if(InviwoApplication::getPtr()){
        ProcessorNetwork* network = InviwoApplication::getPtr()->getProcessorNetwork();
        if(network){
            ProcessorNetworkEvaluator* evaluator = ProcessorNetworkEvaluator::getProcessorNetworkEvaluatorForProcessorNetwork(network);
            if(evaluator){
                evaluator->disableEvaluation();
                Py_RETURN_NONE;
            }
            std::string msg = std::string("disableEvaluation() could not find ProcessorNetworkEvaluator");
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }
        std::string msg = std::string("disableEvaluation() could not find ProcessorNetwork");
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
        
    }
    std::string msg = std::string("disableEvaluation() could not find InviwoApplication");
    PyErr_SetString(PyExc_TypeError, msg.c_str());
    return 0;
}



PyObject* py_enableEvaluation(PyObject* /*self*/, PyObject* /*args*/){
    if(InviwoApplication::getPtr()){
        ProcessorNetwork* network = InviwoApplication::getPtr()->getProcessorNetwork();
        if(network){
            ProcessorNetworkEvaluator* evaluator = ProcessorNetworkEvaluator::getProcessorNetworkEvaluatorForProcessorNetwork(network);
            if(evaluator){
                evaluator->enableEvaluation();
                Py_RETURN_NONE;
            }
            std::string msg = std::string("disableEvaluation() could not find ProcessorNetworkEvaluator");
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }
        std::string msg = std::string("disableEvaluation() could not find ProcessorNetwork");
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;

    }
    std::string msg = std::string("disableEvaluation() could not find InviwoApplication");
    PyErr_SetString(PyExc_TypeError, msg.c_str());
    return 0;
}

PySnapshotMethod::PySnapshotMethod()
    : filename_("filename")
    , canvas_("canvas",true)
{
    addParam(&filename_);
    addParam(&canvas_);
}

PySnapshotCanvasMethod::PySnapshotCanvasMethod()
: canvasID_("canvasID")
, filename_("filename")
{
    addParam(&canvasID_);
    addParam(&filename_);
}

}

