#define IVW_PYINVIWO_CPP

#include "../pythonmodule.h"
#include "pyinviwo.h"

#include "../pythoneditorwidget.h"
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/boolproperty.h>

#include <inviwo/qt/editor/networkeditor.h>
#include <inviwo/qt/widgets/processors/processorwidgetqt.h>

#include <modules/opengl/canvasprocessorgl.h>

#include "../pyvariant.h"

using namespace inviwo;


#include "info.py.h"
#include "properties.py.h"
#include "camera.py.h"
#include "canvas.py.h"
#include "list.py.h"
#include "snapshot.py.h"

static PyObject* py_print(PyObject* /*self*/, PyObject* args) {
    char* msg;
    int len;
    int isStderr;
    if (!PyArg_ParseTuple(args, "s#i", &msg, &len, &isStderr)) {
        LogWarnCustom("inviwo.Python.py_print", "failed to parse log message");
    }
    else {
        std::string message(msg);
        if(message.size()!=0){
            inviwo::PythonEditorWidget::getPythonEditorWidget()->appendToOutput(msg);
        }
    }
    Py_RETURN_NONE;
}



static PyMethodDef inviwo_methods[] = {
    (new PyInfoMethod())->getDef(),
    (new PySetPropertyValueMethod())->getDef(),
    (new PySetPropertyMaxValueMethod())->getDef(),
    (new PySetPropertyMinValueMethod())->getDef(),
    (new PyGetPropertyValueMethod())->getDef(),
    (new PyGetPropertyMaxValueMethod())->getDef(),
    (new PyGetPropertyMinValueMethod())->getDef(),
    (new PySetCameraFocusMethod())->getDef(),
    (new PySetCameraUpMethod())->getDef(),
    (new PySetCameraPosMethod())->getDef(),
    (new PyListPropertiesMethod())->getDef(),
    (new PyListProcessorsMethod())->getDef(),
    (new PyCanvasCountMethod())->getDef(),
    (new PySetViewportMethod())->getDef(),
    (new PySnapshotMethod())->getDef(),
    (new PySnapshotCanvasMethod())->getDef(),
    { NULL, NULL, 0, NULL} // null termination
};

static PyMethodDef internal_methods[] = {
    {"ivwPrint",py_print,METH_VARARGS,"Internal helper function used for accessing script output"},
    { NULL, NULL, 0, NULL} // null termination
};

namespace inviwo{
    PyInviwo::PyInviwo(){
        if (Py_IsInitialized()) {
            if(!Py_InitModule("inviwo_internal",internal_methods)){
                LogWarn("Failed to init python module 'inviwo_internal'");
            }
            LogInfo("Python module 'inviwo_internal' initialized");

            if(!Py_InitModule("inviwo",inviwo_methods)){
                LogWarn("Failed to init python module 'inviwo'");
            }
            LogInfo("Python module 'inviwo' initialized");
            

            PythonScript *outputCatcher = new PythonScript();
            inviwo::InviwoApplicationQt* appQt = dynamic_cast<inviwo::InviwoApplicationQt*>(inviwo::InviwoApplication::getPtr());  
            if(!outputCatcher->load(appQt->getBasePath() +"modules/python/scripts/outputredirector.py",false)){
                LogWarn("Pythin init script failed to load");
            }
            else if(!outputCatcher->run()){
                LogWarn("Pythin init script failed to run");
                LogWarn(outputCatcher->getLog());
            }
        }else{
            LogError("Python environment not initialized");
        }

    }


}//namespace