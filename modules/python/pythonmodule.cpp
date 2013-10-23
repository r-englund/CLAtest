#include <modules/python/pythonmodule.h>

#include <inviwo/core/util/stringconversion.h>

#include "pyinviwo.h"

namespace inviwo {




PythonModule::PythonModule() : InviwoModule() {
    setIdentifier("Python");
    setXMLFileName("python/pythonmodule.xml");   

    pyInviwo_ = new PyInviwo();

}

PythonModule::~PythonModule(){
    delete pyInviwo_;
    Py_Finalize();
}



} // namespace
