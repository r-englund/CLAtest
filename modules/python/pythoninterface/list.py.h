#ifndef IVW_PYLISTMEHTODSINVIWO_H
#define IVW_PYLISTMEHTODSINVIWO_H

#ifndef IVW_PYINVIWO_CPP
    #error This file should only be included from pyinviwo.cpp
#endif

#include <modules/python/pythonmoduledefine.h>

#include "pythonMethod.h"

static PyObject* py_listProperties(PyObject* /*self*/, PyObject* /*args*/);
static PyObject* py_listProcesoors(PyObject* /*self*/, PyObject* /*args*/);

namespace inviwo {
    class PyListPropertiesMethod : public PythonMethod{
    public:
        char *getName(){return "listProperties";}
        char *getDesc(){return "listProperties(processor name). List all properties for a processor.";}
        virtual PyCFunction getFunc(){return py_listProperties;}

    };
    class PyListProcessorsMethod : public PythonMethod{
    public:
        char *getName(){return "listProcessors";}
        char *getDesc(){return "listProcessors(). Lists all processors in the current network.";}
        virtual PyCFunction getFunc(){return py_listProcesoors;}

    };

} //namespace


static PyObject* py_listProperties(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 1) {
        std::ostringstream errStr;
        errStr << "listProperties() takes exactly 1 argument: processor name";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    // check parameter if is string
    if (!PyString_Check(PyTuple_GetItem(args, 0))) {
        PyErr_SetString(PyExc_TypeError, "listProperties() argument must be a string");
        return 0;
    }

    std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));

    Processor* processor = getProcessor(processorName);
    if(!processor){
        std::ostringstream errStr;
        errStr << "listProperties(): no processor with name " << processorName << " could be found";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
    }else{
        std::vector<Property*> props = processor->getProperties();
        for(std::vector<Property*>::const_iterator p = props.begin(); p != props.end();++p){
            std::string name = (*p)->getIdentifier();
            std::string type  = (*p)->getClassName();
            PyRun_SimpleString(("print \""+ name + " : "+ type + "\"").c_str());
        }

    }
    Py_RETURN_NONE;
}



static PyObject* py_listProcesoors(PyObject* /*self*/, PyObject* /*args*/){
    if(InviwoApplication::getPtr() && InviwoApplication::getPtr()->getProcessorNetwork()){
        std::vector<Processor*> processors  = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessors();
        for(std::vector<Processor*>::const_iterator processor = processors.begin();processor!=processors.end();++processor){
            std::string name = (*processor)->getIdentifier();
            std::string type  = (*processor)->getClassName();
            PyRun_SimpleString(("print \""+ name + " : "+ type + "\"").c_str());
        }
    }
    Py_RETURN_NONE;
}


#endif // IVW_PYLISTMEHTODSINVIWO_H


