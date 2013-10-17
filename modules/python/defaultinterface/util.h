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

PyObject* py_closeInviwo(PyObject* /*self*/, PyObject* /*args*/);


class IVW_MODULE_PYTHON_API PySnapshotMethod : public PyMethod{
    public:
        char *getName(){return "snapshot";}
        char *getDesc(){return "snapshot(filename, [canvas])\tSaves a snapshot of the specified canvas to the given file. If no canvas name is passed, the first canvas in the network is chosen.";}
        virtual PyCFunction getFunc(){return py_snapshot;}
};

class IVW_MODULE_PYTHON_API PySnapshotCanvasMethod : public PyMethod{
public:
    char *getName(){return "snapshotCanvas";}
    char *getDesc(){return "snapshotCanvas(i, filename)\tSaves a snapshot of the ith canvas to the given file.";}
    virtual PyCFunction getFunc(){return py_snapshotCanvas;}
};

class IVW_MODULE_PYTHON_API PyGetBasePathMethod : public PyMethod{
public:
    char *getName(){return "getBasePath";}
    char *getDesc(){return "getBasePath()\tReturns the path to Inviwos base folder.";}
    virtual PyCFunction getFunc(){return py_getBasePath;}
};

class IVW_MODULE_PYTHON_API PyGetDataPathMethod : public PyMethod{
public:
    char *getName(){return "getDataPath";}
    char *getDesc(){return "getDataPath()\tReturns the path to Inviwos data folder.";}
    virtual PyCFunction getFunc(){return py_getDataPath;}
};


class IVW_MODULE_PYTHON_API PyGetWorkspaceSavePathMethod : public PyMethod{
public:
    char *getName(){return "getWorkspaceSavePathtReturns the path to Inviwos workspace folder.";}
    char *getDesc(){return "getWorkspaceSavePathtReturns()\tReturns the path to Inviwos workspace folder.";}
    virtual PyCFunction getFunc(){return py_getWorkspaceSavePath;}
};

class IVW_MODULE_PYTHON_API PyGetVolumePathMethod : public PyMethod{
public:
    char *getName(){return "getVolumePath";}
    char *getDesc(){return "getVolumePath()\tReturns the path to Inviwos volume folder.";}
    virtual PyCFunction getFunc(){return py_getVolumePath;}
};

class IVW_MODULE_PYTHON_API PyGetImagePathMethod : public PyMethod{
public:
    char *getName(){return "getImagePath";}
    char *getDesc(){return "getImagePath()\tReturns the path to Inviwos image folder.";}
    virtual PyCFunction getFunc(){return py_getImagePath;}
};

class IVW_MODULE_PYTHON_API PyGetModulePathMethod : public PyMethod{
public:
    char *getName(){return "getModulePath";}
    char *getDesc(){return "getModulePath()\tReturns the path to Inviwo module folder.";}
    virtual PyCFunction getFunc(){return py_getModulePath;}
};



class IVW_MODULE_PYTHON_API PyCloseInviwoMethod : public PyMethod{
public:
    char *getName(){return "closeInviwo";}
    char *getDesc(){return "closeInviwo()\tMethod to close Inviwo.";}
    virtual PyCFunction getFunc(){return py_closeInviwo;}
};

} //namespace



#endif // IVW_PYSNAPSHOTMEHTODINVIWO_H


