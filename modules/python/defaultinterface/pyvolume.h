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

#ifndef IVW_PYVOLUMEINVIWO_H
#define IVW_PYVOLUMEINVIWO_H



#include <modules/python/pythonmoduledefine.h>

#include "../pythoninterface/pymethod.h"

namespace inviwo {


PyObject* py_setVoxel(PyObject* /*self*/, PyObject* args);
PyObject* py_getVolumeDimension(PyObject* /*self*/, PyObject* args);

PyObject* py_saveTransferFunction(PyObject* /*self*/, PyObject* args);
PyObject* py_loadTransferFunction(PyObject* /*self*/, PyObject* args);

PyObject* py_clearTransferfunction(PyObject* /*self*/, PyObject* args);
PyObject* py_addPointTransferFunction(PyObject* /*self*/, PyObject* args);


class IVW_MODULE_PYTHON_API PySetVoxelMethod : public PyMethod{
public:
    PySetVoxelMethod();
    virtual ~PySetVoxelMethod(){}

    virtual std::string getName()const{return "setVoxel";}
    virtual std::string getDesc()const{return "Set voxel value of volume vol, at (x,y,z) ot value v (between 0-1).";}
    virtual PyCFunction getFunc(){return py_setVoxel;}

private:
    PyParamString processor_;
    PyParamString property_;
    PyParamUVec3  voxelPosition_;
    PyParamFloat  voxelValue_;
};


class IVW_MODULE_PYTHON_API PyGetVolumeDimension : public PyMethod{
public:
    PyGetVolumeDimension();
    virtual ~PyGetVolumeDimension(){}

    virtual std::string getName()const{return "getVolumeDimension";}
    virtual std::string getDesc()const{return "Get dimesnion of volume.";}
    virtual PyCFunction getFunc(){return py_getVolumeDimension;}

private:
    PyParamString processor_;
};



class IVW_MODULE_PYTHON_API PySaveTransferFunction : public PyMethod{
public:
    PySaveTransferFunction();
    virtual ~PySaveTransferFunction(){}

    virtual std::string getName()const{return "saveTransferFunction";}
    virtual std::string getDesc()const{return "Save a transfer function to file from the specified transfer function property.";}
    virtual PyCFunction getFunc(){return py_saveTransferFunction;}

private:
    PyParamString processor_;
    PyParamString property_;
    PyParamString filename_;
};



class IVW_MODULE_PYTHON_API PyLoadTransferFunction : public PyMethod{
public:
    PyLoadTransferFunction();
    virtual ~PyLoadTransferFunction(){}

    virtual std::string getName()const{return "loadTransferFunction";}
    virtual std::string getDesc()const{return "Load a transfer function from file into the specified transfer function property.";}
    virtual PyCFunction getFunc(){return py_loadTransferFunction;}

private:
    PyParamString processor_;
    PyParamString property_;
    PyParamString filename_;
};



class IVW_MODULE_PYTHON_API PyClearTransferfunction : public PyMethod{
public:
    PyClearTransferfunction();
    virtual ~PyClearTransferfunction(){}

    virtual std::string getName()const{return "clearTransferfunction";}
    virtual std::string getDesc()const{return "Clears a transfer function.";}
    virtual PyCFunction getFunc(){return py_clearTransferfunction;}

private:
    PyParamString processor_;
    PyParamString property_;
};


class IVW_MODULE_PYTHON_API PyAddTransferFunction : public PyMethod{
public:
    PyAddTransferFunction();
    virtual ~PyAddTransferFunction(){}

    virtual std::string getName()const{return "addPointToTransferFunction";}
    virtual std::string getDesc()const{return "Load a transfer function from file into the specified transfer function property.";}
    virtual PyCFunction getFunc(){return py_addPointTransferFunction;}

private:
    PyParamString processor_;
    PyParamString property_;
    PyParamVec2 pos_;
    PyParamVec3 color_;
};


} //namespace



#endif // IVW_PYSNAPSHOTMEHTODINVIWO_H


