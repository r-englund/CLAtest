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

    pyInviwo_ = new PyInviwo();
    pythonEditorWidget_  = new PythonEditorWidget(win);

}

PythonModule::~PythonModule(){
    delete pythonEditorWidget_;
    delete pyInviwo_;
    Py_Finalize();
}



} // namespace
