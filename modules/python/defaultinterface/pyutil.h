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

PyObject* py_quitInviwo(PyObject* /*self*/, PyObject* /*args*/);


PyObject* py_disableEvaluation(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_enableEvaluation(PyObject* /*self*/, PyObject* /*args*/);


class IVW_MODULE_PYTHON_API PySnapshotMethod : public PyMethod{
    public:
        std::string getName(){return "snapshot";}
        std::string getDesc(){return "snapshot(filename, [canvas])\tSaves a snapshot of the specified canvas to the given file. If no canvas name is passed, the first canvas in the network is chosen.";}
        virtual PyCFunction getFunc(){return py_snapshot;}
};

class IVW_MODULE_PYTHON_API PySnapshotCanvasMethod : public PyMethod{
public:
    std::string getName(){return "snapshotCanvas";}
    std::string getDesc(){return "snapshotCanvas(i, filename)\tSaves a snapshot of the ith canvas to the given file.";}
    virtual PyCFunction getFunc(){return py_snapshotCanvas;}
};

class IVW_MODULE_PYTHON_API PyGetBasePathMethod : public PyMethod{
public:
    std::string getName(){return "getBasePath";}
    std::string getDesc(){return "getBasePath()\tReturns the path to Inviwos base folder.";}
    virtual PyCFunction getFunc(){return py_getBasePath;}
};

class IVW_MODULE_PYTHON_API PyGetDataPathMethod : public PyMethod{
public:
    std::string getName(){return "getDataPath";}
    std::string getDesc(){return "getDataPath()\tReturns the path to Inviwos data folder.";}
    virtual PyCFunction getFunc(){return py_getDataPath;}
};


class IVW_MODULE_PYTHON_API PyGetWorkspaceSavePathMethod : public PyMethod{
public:
    std::string getName(){return "getWorkspaceSavePathtReturns the path to Inviwos workspace folder.";}
    std::string getDesc(){return "getWorkspaceSavePathtReturns()\tReturns the path to Inviwos workspace folder.";}
    virtual PyCFunction getFunc(){return py_getWorkspaceSavePath;}
};

class IVW_MODULE_PYTHON_API PyGetVolumePathMethod : public PyMethod{
public:
    std::string getName(){return "getVolumePath";}
    std::string getDesc(){return "getVolumePath()\tReturns the path to Inviwos volume folder.";}
    virtual PyCFunction getFunc(){return py_getVolumePath;}
};

class IVW_MODULE_PYTHON_API PyGetImagePathMethod : public PyMethod{
public:
    std::string getName(){return "getImagePath";}
    std::string getDesc(){return "getImagePath()\tReturns the path to Inviwos image folder.";}
    virtual PyCFunction getFunc(){return py_getImagePath;}
};

class IVW_MODULE_PYTHON_API PyGetModulePathMethod : public PyMethod{
public:
    std::string getName(){return "getModulePath";}
    std::string getDesc(){return "getModulePath()\tReturns the path to Inviwo module folder.";}
    virtual PyCFunction getFunc(){return py_getModulePath;}
};



class IVW_MODULE_PYTHON_API PyQuitInviwoMethod : public PyMethod{
public:
    std::string getName(){return "quit";}
    std::string getDesc(){return "quit()\tMethod to quit Inviwo.";}
    virtual PyCFunction getFunc(){return py_quitInviwo;}
};



class IVW_MODULE_PYTHON_API PyDisableEvaluation : public PyMethod{
public:
    std::string getName(){return "beginUpdate";}
    std::string getDesc(){return "beginUpdate()\tMethod to disable evaluation of Inviwos network.";}
    virtual PyCFunction getFunc(){return py_disableEvaluation;}
};



class IVW_MODULE_PYTHON_API PyEnableEvaluation : public PyMethod{
public:
    std::string getName(){return "endUpdate";}
    std::string getDesc(){return "endUpdate()\tMethod to reenable evaluation of Inviwos network.";}
    virtual PyCFunction getFunc(){return py_enableEvaluation;}
};

} //namespace



#endif // IVW_PYSNAPSHOTMEHTODINVIWO_H


