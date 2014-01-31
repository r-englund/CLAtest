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

#ifndef IVW_PYCAMERAMEHTODINVIWO_H
#define IVW_PYCAMERAMEHTODINVIWO_H


#include <modules/python/pythonmoduledefine.h>

#include "../pythoninterface/pymethod.h"

namespace inviwo {
PyObject* py_setCameraFocus(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_setCameraUp(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_setCameraPos(PyObject* /*self*/, PyObject* /*args*/);


    class IVW_MODULE_PYTHON_API PySetCameraFocusMethod : public PyMethod{
    public:
        PySetCameraFocusMethod();
        virtual ~PySetCameraFocusMethod(){}

        virtual std::string getName()const{return "setCameraFocus";}
        virtual std::string getDesc()const{return "Function to set the cameras focal point.";}
        virtual PyCFunction getFunc(){return py_setCameraFocus;}
    private:
        PyParamString processor_;
        PyParamString property_;
        PyParamVec3   focusPoint_;
    };

    class IVW_MODULE_PYTHON_API PySetCameraUpMethod : public PyMethod{
    public:
        PySetCameraUpMethod();
        virtual ~PySetCameraUpMethod(){}

        virtual std::string getName()const{return "setCameraUp";}
        virtual std::string getDesc()const{return "Function to set the cameras up direction.";}
        virtual PyCFunction getFunc(){return py_setCameraUp;}
    private:
        PyParamString processor_;
        PyParamString property_;
        PyParamVec3   upVector_;

    };

    class IVW_MODULE_PYTHON_API PySetCameraPosMethod : public PyMethod{
    public:
        PySetCameraPosMethod();
        virtual ~PySetCameraPosMethod(){}

        virtual std::string getName()const{return "setCameraPosition";}
        virtual std::string getDesc()const{return "Function to set the cameras position.";}
        virtual PyCFunction getFunc(){return py_setCameraPos;}
    private:
        PyParamString processor_;
        PyParamString property_;
        PyParamVec3   position_;

    };

} //namespace





#endif // IVW_PYCAMERAMEHTODINVIWO_H


