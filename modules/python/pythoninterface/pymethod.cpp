#include "../pythoninterface/pymethod.h"

#include "pymodule.h"

namespace inviwo{


PyMethod::PyMethod(){
}


PyMethodDef* PyMethod::getDef(){
    def_.ml_doc   = getDesc();
    def_.ml_flags = getFlags();
    def_.ml_meth  = getFunc();
    def_.ml_name  = getName();
    return &def_;
}




}//namespace