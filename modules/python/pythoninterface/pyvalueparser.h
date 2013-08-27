#ifndef IVW_PYVALUEPARSERINVIWO_H
#define IVW_PYVALUEPARSERINVIWO_H

#include <modules/python/pythonmoduledefine.h>


#ifdef _DEBUG
#undef _DEBUG //Prevent linking debug build of python
#include <Python.h>
#define _DEBUG 1
#else
#include <Python.h>
#endif

#include <vector>

#include <inviwo/core/properties/properties.h>

namespace inviwo {

    class IVW_MODULE_PYTHON_API PyValueParser{
    
    public:
        PyValueParser(){}

        template <typename T> T parse(PyObject *arg);
        template <typename T> PyObject* toPyObject(T t);
        void setProperty(Property *p,PyObject *args);
        PyObject *getProperty(Property *p);

    };
} //namespace

#endif // IVW_PYVALUEPARSERINVIWO_H
