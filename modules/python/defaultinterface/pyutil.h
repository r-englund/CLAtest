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

#ifndef IVW_PYSNAPSHOTMEHTODINVIWO_H
#define IVW_PYSNAPSHOTMEHTODINVIWO_H


#include <modules/python/pythonmoduledefine.h>

#include "../pythoninterface/pymethod.h"

namespace inviwo {

PyObject* py_snapshot(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_snapshotCanvas(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getBasePath(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getDataPath(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getWorkspaceSavePath(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getVolumePath(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getImagePath(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getModulePath(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getTransferFunctionPath(PyObject* /*self*/, PyObject* /*args*/);

PyObject* py_quitInviwo(PyObject* /*self*/, PyObject* /*args*/);


PyObject* py_disableEvaluation(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_enableEvaluation(PyObject* /*self*/, PyObject* /*args*/);


class IVW_MODULE_PYTHON_API PySnapshotMethod : public PyMethod{
    public:
        PySnapshotMethod();
        virtual ~PySnapshotMethod(){}
        virtual std::string getName()const{return "snapshot";}
        virtual std::string getDesc()const{return "Saves a snapshot of the specified canvas to the given file. If no canvas name is passed, the first canvas in the network is chosen.";}
        virtual PyCFunction getFunc(){return py_snapshot;}
private:
    PyParamString filename_;
    PyParamString canvas_;
};

class IVW_MODULE_PYTHON_API PySnapshotCanvasMethod : public PyMethod{
public:
    PySnapshotCanvasMethod();
    virtual ~PySnapshotCanvasMethod(){}
    virtual std::string getName()const{return "snapshotCanvas";}
    virtual std::string getDesc()const{return "Saves a snapshot of the ith canvas to the given file.";}
    virtual PyCFunction getFunc(){return py_snapshotCanvas;}
private:
    PyParamInt canvasID_;
    PyParamString filename_;
};

class IVW_MODULE_PYTHON_API PyGetBasePathMethod : public PyMethod{
public:
    virtual std::string getName()const{return "getBasePath";}
    virtual std::string getDesc()const{return "Returns the path to Inviwos base folder.";}
    virtual PyCFunction getFunc(){return py_getBasePath;}
};

class IVW_MODULE_PYTHON_API PyGetDataPathMethod : public PyMethod{
public:
    virtual std::string getName()const{return "getDataPath";}
    virtual std::string getDesc()const{return "Returns the path to Inviwos data folder.";}
    virtual PyCFunction getFunc(){return py_getDataPath;}
};


class IVW_MODULE_PYTHON_API PyGetWorkspaceSavePathMethod : public PyMethod{
public:
    virtual std::string getName()const{return "getWorkspaceSavePatht";}
    virtual std::string getDesc()const{return "Returns the path to Inviwos workspace folder.";}
    virtual PyCFunction getFunc(){return py_getWorkspaceSavePath;}
};

class IVW_MODULE_PYTHON_API PyGetVolumePathMethod : public PyMethod{
public:
    virtual std::string getName()const{return "getVolumePath";}
    virtual std::string getDesc()const{return "Returns the path to Inviwos volume folder.";}
    virtual PyCFunction getFunc(){return py_getVolumePath;}
};

class IVW_MODULE_PYTHON_API PyGetImagePathMethod : public PyMethod{
public:
    virtual std::string getName()const{return "getImagePath";}
    virtual std::string getDesc()const{return "Returns the path to Inviwos image folder.";}
    virtual PyCFunction getFunc(){return py_getImagePath;}
};

class IVW_MODULE_PYTHON_API PyGetModulePathMethod : public PyMethod{
public:
    virtual std::string getName()const{return "getModulePath";}
    virtual std::string getDesc()const{return "Returns the path to Inviwo module folder.";}
    virtual PyCFunction getFunc(){return py_getModulePath;}
};


class IVW_MODULE_PYTHON_API PyGetTransferFunctionPath : public PyMethod{
public:
    virtual std::string getName()const{return "getTransferFunctionPath";}
    virtual std::string getDesc()const{return "Returns the path to Inviwo transfer function folder.";}
    virtual PyCFunction getFunc(){return py_getTransferFunctionPath;}
};



class IVW_MODULE_PYTHON_API PyQuitInviwoMethod : public PyMethod{
public:
    virtual std::string getName()const{return "quit";}
    virtual std::string getDesc()const{return "Method to quit Inviwo.";}
    virtual PyCFunction getFunc(){return py_quitInviwo;}
};



class IVW_MODULE_PYTHON_API PyDisableEvaluation : public PyMethod{
public:
    virtual std::string getName()const{return "beginUpdate";}
    virtual std::string getDesc()const{return "Method to disable evaluation of Inviwos network.";}
    virtual PyCFunction getFunc(){return py_disableEvaluation;}
};



class IVW_MODULE_PYTHON_API PyEnableEvaluation : public PyMethod{
public:
    virtual std::string getName()const{return "endUpdate";}
    virtual std::string getDesc()const{return "Method to reenable evaluation of Inviwos network.";}
    virtual PyCFunction getFunc(){return py_enableEvaluation;}
};

} //namespace



#endif // IVW_PYSNAPSHOTMEHTODINVIWO_H


