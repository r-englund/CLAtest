#ifndef IVW_INFOMEHTODINVIWO_H
#define IVW_INFOMEHTODINVIWO_H

#ifndef IVW_PYINVIWO_CPP
    #error This file should only be included from pyinviwo.cpp
#endif

#include <modules/python/pythonmoduledefine.h>

#include "pythonMethod.h"

static PyObject* py_info(PyObject* /*self*/, PyObject* /*args*/);

namespace inviwo {
    class PyInfoMethod : public PythonMethod{
    public:
        char *getName(){return "info";}
        char *getDesc(){return "info()\tPrints documentation of the module's functions.";}
        virtual PyCFunction getFunc(){return py_info;}

        static void printAllDescriptions(){
            //std::stringstream ss;
            for(int i = 0;i<methods_.size();i++){
                std::string msg = "print \"";
                msg += methods_[i]->getDesc();
                msg += "\" ";
                PyRun_SimpleString(msg.c_str());
                PyRun_SimpleString("print \" \"");
            }
            //return ss.str();
        }

    };

} //namespace

static PyObject* py_info(PyObject* /*self*/, PyObject* /*args*/){
    inviwo::PyInfoMethod::printAllDescriptions();
   // PyRun_SimpleString(("print \""+ s + "\"").c_str());
    Py_RETURN_NONE;
}


#endif // IVW_INFOMEHTODINVIWO_H


