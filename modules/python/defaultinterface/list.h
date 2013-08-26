#ifndef IVW_PYLISTMEHTODSINVIWO_H
#define IVW_PYLISTMEHTODSINVIWO_H



#include <modules/python/pythonmoduledefine.h>

#include "../pythoninterface/pymethod.h"

PyObject* py_listProperties(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_listProcesoors(PyObject* /*self*/, PyObject* /*args*/);

namespace inviwo {
    class PyListPropertiesMethod : public PyMethod{
    public:
        char *getName(){return "listProperties";}
        char *getDesc(){return "listProperties(processor name)\tList all properties for a processor.";}
        virtual PyCFunction getFunc(){return py_listProperties;}

    };
    class PyListProcessorsMethod : public PyMethod{
    public:
        char *getName(){return "listProcessors";}
        char *getDesc(){return "listProcessors()\tLists all processors in the current network.";}
        virtual PyCFunction getFunc(){return py_listProcesoors;}

    };

} //namespace


#endif // IVW_PYLISTMEHTODSINVIWO_H


