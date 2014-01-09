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


class IVW_MODULE_PYTHON_API PySetVoxelMethod : public PyMethod{
public:
    std::string getName(){return "setVoxel";}
    std::string getDesc(){return "setVoxel(vol,(x,y,z),v)\tSet voxel value of volume vol, at (x,y,z) ot value v (between 0-1).";}
    virtual PyCFunction getFunc(){return py_setVoxel;}
};


class IVW_MODULE_PYTHON_API PyGetVolumeDimension : public PyMethod{
public:
    std::string getName(){return "getVolumeDimension";}
    std::string getDesc(){return "getVolumeDimension(vol)\tGet dimesnion of volume.";}
    virtual PyCFunction getFunc(){return py_getVolumeDimension;}
};

} //namespace



#endif // IVW_PYSNAPSHOTMEHTODINVIWO_H


