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
        char *getName(){return "setPropertyValue";}
        char *getDesc(){return "setPropertyValue(processor name, property id, scalar or tuple)\tAssigns a value to a processor property. The value has to be passed as scalar or tuple, depending on the property's cardinality. Camera properties take a 3-tuple of 3-tuples, containing the position, focus and up vectors. Option properties expect an option key.";}
        virtual PyCFunction getFunc(){return py_setPropertyValue;}
    };

    class IVW_MODULE_PYTHON_API PySetPropertyMaxValueMethod : public PyMethod{
    public:
        char *getName(){return "setPropertyMaxValue";}
        char *getDesc(){return "setPropertyValue(processor name, property id, scalar or tuple)\tDefines the max value for a property.";}
        virtual PyCFunction getFunc(){return py_setPropertyMaxValue;}
    };

    class IVW_MODULE_PYTHON_API PySetPropertyMinValueMethod : public PyMethod{
    public:
        char *getName(){return "setPropertyMinValue";}
        char *getDesc(){return "setPropertyMinValue(processor name, property id, scalar or tuple)\tDefines the min value for a property.";}
        virtual PyCFunction getFunc(){return py_setPropertyMinValue;}
    };

    class IVW_MODULE_PYTHON_API PyGetPropertyValueMethod : public PyMethod{
    public:
        char *getName(){return "getPropertyValue";}
        char *getDesc(){return "getPropertyValue(processor name, property id)\tReturns the current value of a processor property (scalar or tuple).";}
        virtual PyCFunction getFunc(){return py_getPropertyValue;}
    };

    class IVW_MODULE_PYTHON_API PyGetPropertyMaxValueMethod : public PyMethod{
    public:
        char *getName(){return "getPropertyMaxValue";}
        char *getDesc(){return "getPropertyValue(processor name, property id)\tReturns the max value for a property (scalar or tuple).";}
        virtual PyCFunction getFunc(){return py_getPropertyMaxValue;}
    };

    class IVW_MODULE_PYTHON_API PyGetPropertyMinValueMethod : public PyMethod{
    public:
        char *getName(){return "getPropertyMinValue";}
        char *getDesc(){return "getPropertyValue(processor name, property id)\tReturns the min value for a property (scalar or tuple).";}
        virtual PyCFunction getFunc(){return py_getPropertyMinValue;}
    };

    class IVW_MODULE_PYTHON_API PyClickButtonMethod : public PyMethod{
    public:
        char *getName(){return "clickButton";}
        char *getDesc(){return "clickButton(processor name, buttonProperty id)\t Simulates a click on a button property.";}
        virtual PyCFunction getFunc(){return py_clickButton;}
    };
    
} //namespace



#endif // IVW_PROPERTIESMEHTODSINVIWO_H


