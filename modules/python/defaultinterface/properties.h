#ifndef IVW_PROPERTIESMEHTODSINVIWO_H
#define IVW_PROPERTIESMEHTODSINVIWO_H



#include <modules/python/pythonmoduledefine.h>

#include "../pythoninterface/pymethod.h"



PyObject* py_setPropertyValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_setPropertyMaxValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_setPropertyMinValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getPropertyValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getPropertyMaxValue(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getPropertyMinValue(PyObject* /*self*/, PyObject* /*args*/);

namespace inviwo {
    class PySetPropertyValueMethod : public PyMethod{
    public:
        char *getName(){return "setPropertyValue";}
        char *getDesc(){return "setPropertyValue(processor name, property id, scalar or tuple)\tAssigns a value to a processor property. The value has to be passed as scalar or tuple, depending on the property's cardinality. Camera properties take a 3-tuple of 3-tuples, containing the position, focus and up vectors. Option properties expect an option key.";}
        virtual PyCFunction getFunc(){return py_setPropertyValue;}
    };

    class PySetPropertyMaxValueMethod : public PyMethod{
    public:
        char *getName(){return "setPropertyMaxValue";}
        char *getDesc(){return "setPropertyValue(processor name, property id, scalar or tuple)\tDefines the max value for a property.";}
        virtual PyCFunction getFunc(){return py_setPropertyMaxValue;}
    };

    class PySetPropertyMinValueMethod : public PyMethod{
    public:
        char *getName(){return "setPropertyMinValue";}
        char *getDesc(){return "setPropertyMinValue(processor name, property id, scalar or tuple)\tDefines the min value for a property.";}
        virtual PyCFunction getFunc(){return py_setPropertyMinValue;}
    };

    class PyGetPropertyValueMethod : public PyMethod{
    public:
        char *getName(){return "getPropertyValue";}
        char *getDesc(){return "getPropertyValue(processor name, property id)\tReturns the current value of a processor property (scalar or tuple).";}
        virtual PyCFunction getFunc(){return py_getPropertyValue;}
    };

    class PyGetPropertyMaxValueMethod : public PyMethod{
    public:
        char *getName(){return "getPropertyMaxValue";}
        char *getDesc(){return "getPropertyValue(processor name, property id)\tReturns the max value for a property (scalar or tuple).";}
        virtual PyCFunction getFunc(){return py_getPropertyMaxValue;}
    };

    class PyGetPropertyMinValueMethod : public PyMethod{
    public:
        char *getName(){return "getPropertyMinValue";}
        char *getDesc(){return "getPropertyValue(processor name, property id)\tReturns the min value for a property (scalar or tuple).";}
        virtual PyCFunction getFunc(){return py_getPropertyMinValue;}
    };

} //namespace



#endif // IVW_PROPERTIESMEHTODSINVIWO_H


