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

#ifndef IVW_PROPERTIESMEHTODSINVIWO_H
#define IVW_PROPERTIESMEHTODSINVIWO_H



#include <modules/python/pythonmoduledefine.h>

#include "../pythoninterface/pymethod.h"


namespace inviwo {

PyObject* py_setPropertyValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_setPropertyMaxValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_setPropertyMinValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getPropertyValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getPropertyMaxValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getPropertyMinValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_clickButton(PyObject* /*self*/, PyObject* /*args*/);

    class IVW_MODULE_PYTHON_API PySetPropertyValueMethod : public PyMethod{
    public:
        PySetPropertyValueMethod();
        virtual ~PySetPropertyValueMethod(){}

        virtual std::string getName()const{return "setPropertyValue";}
        virtual std::string getDesc()const{return "Assigns a value to a processor property. The value has to be passed as scalar or tuple, depending on the property's cardinality. Camera properties take a 3-tuple of 3-tuples, containing the position, focus and up vectors. Option properties expect an option key.";}
        virtual PyCFunction getFunc(){return py_setPropertyValue;}
    private:
        PyParamString processor_;
        PyParamString property_;
        PyParamVarious value_;
    };

    class IVW_MODULE_PYTHON_API PySetPropertyMaxValueMethod : public PyMethod{
    public:
        PySetPropertyMaxValueMethod();
        virtual ~PySetPropertyMaxValueMethod(){}

        virtual std::string getName()const{return "setPropertyMaxValue";}
        virtual std::string getDesc()const{return "Defines the max value for a property.";}
        virtual PyCFunction getFunc(){return py_setPropertyMaxValue;}
    private:
        PyParamString processor_;
        PyParamString property_;
        PyParamVarious maxValue_;
    };

    class IVW_MODULE_PYTHON_API PySetPropertyMinValueMethod : public PyMethod{
    public:
        PySetPropertyMinValueMethod();
        virtual ~PySetPropertyMinValueMethod(){}

        virtual std::string getName()const{return "setPropertyMinValue";}
        virtual std::string getDesc()const{return "Defines the min value for a property.";}
        virtual PyCFunction getFunc(){return py_setPropertyMinValue;}
    private:
        PyParamString processor_;
        PyParamString property_;
        PyParamVarious minValue_;
    };

    class IVW_MODULE_PYTHON_API PyGetPropertyValueMethod : public PyMethod{
    public:
        PyGetPropertyValueMethod();
        virtual ~PyGetPropertyValueMethod(){}

        virtual std::string getName()const{return "getPropertyValue";}
        virtual std::string getDesc()const{return "Returns the current value of a processor property (scalar or tuple).";}
        virtual PyCFunction getFunc(){return py_getPropertyValue;}
    private:
        PyParamString processor_;
        PyParamString property_;
    };

    class IVW_MODULE_PYTHON_API PyGetPropertyMaxValueMethod : public PyMethod{
    public:
        PyGetPropertyMaxValueMethod();
        virtual ~PyGetPropertyMaxValueMethod(){}

        virtual std::string getName()const{return "getPropertyMaxValue";}
        virtual std::string getDesc()const{return "Returns the max value for a property (scalar or tuple).";}
        virtual PyCFunction getFunc(){return py_getPropertyMaxValue;}
    private:
        PyParamString processor_;
        PyParamString property_;
    };

    class IVW_MODULE_PYTHON_API PyGetPropertyMinValueMethod : public PyMethod{
    public:
        PyGetPropertyMinValueMethod();
        virtual ~PyGetPropertyMinValueMethod(){}

        virtual std::string getName()const{return "getPropertyMinValue";}
        virtual std::string getDesc()const{return "Returns the min value for a property (scalar or tuple).";}
        virtual PyCFunction getFunc(){return py_getPropertyMinValue;}
    private:
        PyParamString processor_;
        PyParamString property_;
    };

    class IVW_MODULE_PYTHON_API PyClickButtonMethod : public PyMethod{
    public:
        PyClickButtonMethod();
        virtual ~PyClickButtonMethod(){}

        virtual std::string getName()const{return "clickButton";}
        virtual std::string getDesc()const{return "Simulates a click on a button property.";}
        virtual PyCFunction getFunc(){return py_clickButton;}
    private:
        PyParamString processor_;
        PyParamString property_;
    };
    
} //namespace



#endif // IVW_PROPERTIESMEHTODSINVIWO_H


