#include <modules/python/pythonmodule.h>

#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/core/common/inviwoapplication.h>

#include <inviwo/core/util/stringconversion.h>

#include "pythoneditorwidget.h"
#include "pyinviwo.h"

namespace inviwo {




PythonModule::PythonModule() : InviwoModule() {
    setIdentifier("Python");
    setXMLFileName("python/pythonmodule.xml");   

    InviwoApplicationQt* appQt = static_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());    
    InviwoMainWindow* win = static_cast<InviwoMainWindow*>(appQt->getMainWindow());

    LogInfo("Python version: " + toString(Py_GetVersion()));
    
    Py_SetProgramName("PyInviwo");

#ifdef WIN32
    Py_NoSiteFlag = 1;  
#endif

    Py_InitializeEx(false);
    if(!Py_IsInitialized()){
        LogError("Python is not Initialized");
        return;
    }
    PyEval_InitThreads();

    addModulePath(appQt->getBasePath() + "modules/python/scripts");
    
    pyInviwo_ = new PyInviwo();
    pythonEditorWidget_  = new PythonEditorWidget(win);
}

PythonModule::~PythonModule(){
    delete pythonEditorWidget_;
    delete pyInviwo_;
    Py_Finalize();
}

void PythonModule::addModulePath(const std::string& path) {

    if (!Py_IsInitialized()) {
        LogWarn("addModulePath(): not initialized");
        return;
    }

    std::string pathConv = path;
    replaceInString(pathConv, "\\", "/");

    LogInfo("Adding '" + pathConv + "' to Python module search path");
    std::string runString = "import sys\n";
    runString.append(std::string("sys.path.append('") + pathConv + std::string("')"));
    int ret = PyRun_SimpleString(runString.c_str());
    if (ret != 0)
        LogWarn("Failed to add '" + pathConv + "' to Python module search path");
}

} // namespace
