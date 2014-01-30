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
    std::string getName(){return "setVoxel";}
    std::string getDesc(){return "Set voxel value of volume vol, at (x,y,z) ot value v (between 0-1).";}
    virtual PyCFunction getFunc(){return py_setVoxel;}
};


class IVW_MODULE_PYTHON_API PyGetVolumeDimension : public PyMethod{
public:
    std::string getName(){return "getVolumeDimension";}
    std::string getDesc(){return "Get dimesnion of volume.";}
    virtual PyCFunction getFunc(){return py_getVolumeDimension;}
};



class IVW_MODULE_PYTHON_API PySaveTransferFunction : public PyMethod{
public:
    std::string getName(){return "saveTransferFunction";}
    std::string getDesc(){return "Save a transfer function to file from the specified transfer function property.";}
    virtual PyCFunction getFunc(){return py_saveTransferFunction;}
};



class IVW_MODULE_PYTHON_API PyLoadTransferFunction : public PyMethod{
public:
    std::string getName(){return "loadTransferFunction";}
    std::string getDesc(){return "Load a transfer function from file into the specified transfer function property.";}
    virtual PyCFunction getFunc(){return py_loadTransferFunction;}
};



class IVW_MODULE_PYTHON_API PyClearTransferfunction : public PyMethod{
public:
    std::string getName(){return "clearTransferfunction";}
    std::string getDesc(){return "Clears a transfer function.";}
    virtual PyCFunction getFunc(){return py_clearTransferfunction;}
};


class IVW_MODULE_PYTHON_API PyAddTransferFunction : public PyMethod{
public:
    PyAddTransferFunction()
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
    std::string getName(){return "addPointToTransferFunction";}
    std::string getDesc(){return "Load a transfer function from file into the specified transfer function property.";}
    virtual PyCFunction getFunc(){return py_addPointTransferFunction;}

private:
    PyParamString processor_;
    PyParamString property_;
    PyParamVec2 pos_;
    PyParamVec4 color_;
};


} //namespace



#endif // IVW_PYSNAPSHOTMEHTODINVIWO_H


