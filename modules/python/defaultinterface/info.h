#ifndef IVW_INFOMEHTODINVIWO_H
#define IVW_INFOMEHTODINVIWO_H


#include <modules/python/pythonmoduledefine.h>

#include "../pythoninterface/pymethod.h"

PyObject* py_info(PyObject* /*self*/, PyObject* /*args*/);

namespace inviwo {
    class PyInfoMethod : public PyMethod{
    public:
        char *getName(){return "info";}
        char *getDesc(){return "info()\tPrints documentation of the module's functions.";}
        virtual PyCFunction getFunc(){return py_info;}

        static void printAllDescriptions(){
            for(int i = 0;i<methods_.size();i++){
                std::string msg = "print \"";
                msg += methods_[i]->getDesc();
                msg += "\" ";
                PyRun_SimpleString(msg.c_str());
                PyRun_SimpleString("print \" \"");
            }
        }

    };

} //namespace


#endif // IVW_INFOMEHTODINVIWO_H


