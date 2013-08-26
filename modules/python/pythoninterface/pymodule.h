#ifndef IVW_PYMODULE_H
#define IVW_PYMODULE_H

#include <modules/python/pythonmoduledefine.h>

#include "../pythoninterface/pymethod.h"

#include <vector>
#include <string>

PyObject* py_info(PyObject* /*self*/, PyObject* /*args*/);

namespace inviwo {

    class IVW_MODULE_PYTHON_API PyInfoMethod : public PyMethod{
    public:
        char *getName(){return "info";}
        char *getDesc(){return "info()\tPrints documentation of the module's functions.";}
        virtual PyCFunction getFunc(){return py_info;}
    };  



class IVW_MODULE_PYTHON_API PyModule{
    std::string moduleName_;
    std::vector<PyMethod*> methods_;
    PyObject* moduleObject_;
    PyObject* dict_;
    PyObject *name_;

    static std::map<PyObject*,PyModule*> instances_;

public:
    PyModule(std::string moduleName,std::vector<PyMethod*> methods = std::vector<PyMethod*>());
    void addMethod(PyMethod* method);
    
    const char* getModuleName();
    
    void printInfo();
    
    static PyModule* getModuleByPyObject(PyObject *obj);


};



}//namespace

#endif