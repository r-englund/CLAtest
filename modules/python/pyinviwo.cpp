#include "pythonmodule.h"
#include "pyinviwo.h"

#include "pythoneditorwidget.h"
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/boolproperty.h>

#include <inviwo/qt/editor/networkeditor.h>
#include <inviwo/qt/widgets/processors/processorwidgetqt.h>

#include <modules/opengl/canvasprocessorgl.h>

#include <inviwo/core/util/variant.h>

using namespace inviwo;

static Processor* getProcessor(std::string id){
    InviwoApplicationQt* appQt = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());  
    return appQt->getProcessorNetwork()->getProcessorByName(id);
}


static PyObject* printModuleInfo(const std::string& moduleName, bool omitFunctionName,
    int spacing, bool collapse, bool blanklines) {

        // import apihelper.py
        PyObject* apihelper = PyImport_ImportModule("apihelper");
        if (!apihelper) {
            PyErr_SetString(PyExc_SystemError,
                (moduleName + std::string(".info() apihelper module not found")).c_str());
            return 0;
        }

        // get reference to info function
        PyObject* func = PyDict_GetItemString(PyModule_GetDict(apihelper), "info");
        if (!func) {
            PyErr_SetString(PyExc_SystemError,
                (moduleName + std::string(".info() apihelper.info() not found")).c_str());
            Py_XDECREF(apihelper);
            return 0;
        }

        // get reference to module
        PyObject* mod = PyImport_AddModule(const_cast<char*>(moduleName.c_str())); // const cast required for
        // Python 2.4
        if (!mod) {
            PyErr_SetString(PyExc_SystemError,
                (moduleName + std::string(".info() failed to access module ") + moduleName).c_str());
            Py_XDECREF(apihelper);
        }

        // build parameter tuple
        std::string docStr = "Module " + moduleName;
        PyObject* arg = Py_BuildValue("(O,s,i,i,i,i)", mod, docStr.c_str(),
            int(omitFunctionName), spacing, (int)collapse, (int)blanklines);
        if (!arg) {
            PyErr_SetString(PyExc_SystemError,
                (moduleName + std::string(".info() failed to create arguments")).c_str());
            Py_XDECREF(apihelper);
            return 0;
        }

        PyObject* callObj = PyObject_CallObject(func, arg);
        bool success = (callObj != 0);

        Py_XDECREF(callObj);
        Py_XDECREF(arg);
        Py_XDECREF(apihelper);

        if (success)
            Py_RETURN_NONE;
        else
            return 0;
}

static PyObject* inviwo_print(PyObject* /*self*/, PyObject* args) {
    char* msg;
    int len;
    int isStderr;
    if (!PyArg_ParseTuple(args, "s#i", &msg, &len, &isStderr)) {
        LogWarnCustom("inviwo.Python.inviwo_print", "failed to parse log message");
    }
    else {
        if (len > 1 || ((len == 1) && (msg[0] != '\0') && (msg[0] != '\r') && (msg[0] != '\n'))) {
            std::string message(msg);
            inviwo::PythonEditorWidget::getPythonEditorWidget()->appendToOutput(msg);
        }
    }

    Py_RETURN_NONE;
}




static PyObject* inviwo_info(PyObject* /*self*/, PyObject* /*args*/){
    return printModuleInfo("inviwo",  true, 0, false, true);
}


static PyObject* inviwo_setPropertyValue(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 3) {
        std::ostringstream errStr;
        errStr << "setPropertyValue() takes exactly 3 arguments: processor name, property id, value";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    // check parameter 1 and 2, if they are strings
    if (!PyString_Check(PyTuple_GetItem(args, 0)) || !PyString_Check(PyTuple_GetItem(args, 1))) {
        PyErr_SetString(PyExc_TypeError, "setPropertyValue() arguments 1 and 2 must be strings");
        return 0;
    }

    std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));
    PyObject* parameter = PyTuple_GetItem(args, 2);

    Processor* processor = getProcessor(processorName);
    if(!processor){
        std::string msg = std::string("setPropertyValue() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("setPropertyValue() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    const std::string className = theProperty->getClassName();
    if(className == "ButtonProperty"){
        ButtonProperty* button = dynamic_cast<ButtonProperty*>(theProperty);
    }else{
        Variant parameterVariant(parameter, theProperty->getVariantType());
        theProperty->setVariant(parameterVariant);
        
    }


    Py_RETURN_NONE;
}

static PyObject* inviwo_setPropertyMaxValue(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 3) {
        std::ostringstream errStr;
        errStr << "setPropertyMaxValue() takes exactly 3 arguments: processor name, property id, value";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    // check parameter 1 and 2, if they are strings
    if (!PyString_Check(PyTuple_GetItem(args, 0)) || !PyString_Check(PyTuple_GetItem(args, 1))) {
        PyErr_SetString(PyExc_TypeError, "setPropertyMaxValue() arguments 1 and 2 must be strings");
        return 0;
    }

    std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));
    PyObject* parameter = PyTuple_GetItem(args, 2);

    Processor* processor = getProcessor(processorName);
    if(!processor){
        std::string msg = std::string("setPropertyMaxValue() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("setPropertyMaxValue() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    auto ordinalFloat = dynamic_cast<OrdinalProperty<float>*>(theProperty);
    auto ordinalInt   = dynamic_cast<OrdinalProperty<int>  *>(theProperty);
    auto ordinalIvec2 = dynamic_cast<OrdinalProperty<ivec2>*>(theProperty);
    auto ordinalIvec3 = dynamic_cast<OrdinalProperty<ivec3>*>(theProperty);
    auto ordinalIvec4 = dynamic_cast<OrdinalProperty<ivec4>*>(theProperty);
    auto ordinalMat2  = dynamic_cast<OrdinalProperty<mat2> *>(theProperty);
    auto ordinalMat3  = dynamic_cast<OrdinalProperty<mat3> *>(theProperty);
    auto ordinalMat4  = dynamic_cast<OrdinalProperty<mat4> *>(theProperty);
    auto ordinalVec2  = dynamic_cast<OrdinalProperty<vec2> *>(theProperty);
    auto ordinalVec3  = dynamic_cast<OrdinalProperty<vec3> *>(theProperty);
    auto ordinalVec4  = dynamic_cast<OrdinalProperty<vec4> *>(theProperty);

    Variant parameterVariant(parameter, theProperty->getVariantType());
    if(ordinalFloat){
        ordinalFloat->setMaxValue(parameterVariant.getFloat());
    }else if(ordinalInt){
        ordinalInt->setMaxValue(parameterVariant.getInt());
    }else if(ordinalIvec2){
        ordinalIvec2->setMaxValue(parameterVariant.getIVec2());
    }else if(ordinalIvec3){
        ordinalIvec3->setMaxValue(parameterVariant.getIVec3());
    }else if(ordinalIvec4){
        ordinalIvec4->setMaxValue(parameterVariant.getIVec4());
    }else if(ordinalMat2){
        ordinalMat2->setMaxValue(parameterVariant.getMat2());
    }else if(ordinalMat3){
        ordinalMat3->setMaxValue(parameterVariant.getMat3());
    }else if(ordinalMat4){
        ordinalMat4->setMaxValue(parameterVariant.getMat4());
    }else if(ordinalVec2){
        ordinalVec2->setMaxValue(parameterVariant.getVec2());
    }else if(ordinalVec3){
        ordinalVec3->setMaxValue(parameterVariant.getVec3());
    }else if(ordinalVec4){
        ordinalVec4->setMaxValue(parameterVariant.getVec4());
    }else{
        LogErrorCustom("inviwo_setPropertyMaxValue","Unknown parameter type");
    }
    
    Py_RETURN_NONE;
}



static PyObject* inviwo_setPropertyMinValue(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 3) {
        std::ostringstream errStr;
        errStr << "setPropertyMinValue() takes exactly 3 arguments: processor name, property id, value";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    // check parameter 1 and 2, if they are strings
    if (!PyString_Check(PyTuple_GetItem(args, 0)) || !PyString_Check(PyTuple_GetItem(args, 1))) {
        PyErr_SetString(PyExc_TypeError, "setPropertyMinValue() arguments 1 and 2 must be strings");
        return 0;
    }

    std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));
    PyObject* parameter = PyTuple_GetItem(args, 2);

    Processor* processor = getProcessor(processorName);
    if(!processor){
        std::string msg = std::string("setPropertyMinValue() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("setPropertyMinValue() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    auto ordinalFloat = dynamic_cast<OrdinalProperty<float>*>(theProperty);
    auto ordinalInt   = dynamic_cast<OrdinalProperty<int>  *>(theProperty);
    auto ordinalIvec2 = dynamic_cast<OrdinalProperty<ivec2>*>(theProperty);
    auto ordinalIvec3 = dynamic_cast<OrdinalProperty<ivec3>*>(theProperty);
    auto ordinalIvec4 = dynamic_cast<OrdinalProperty<ivec4>*>(theProperty);
    auto ordinalMat2  = dynamic_cast<OrdinalProperty<mat2> *>(theProperty);
    auto ordinalMat3  = dynamic_cast<OrdinalProperty<mat3> *>(theProperty);
    auto ordinalMat4  = dynamic_cast<OrdinalProperty<mat4> *>(theProperty);
    auto ordinalVec2  = dynamic_cast<OrdinalProperty<vec2> *>(theProperty);
    auto ordinalVec3  = dynamic_cast<OrdinalProperty<vec3> *>(theProperty);
    auto ordinalVec4  = dynamic_cast<OrdinalProperty<vec4> *>(theProperty);

    Variant parameterVariant(parameter, theProperty->getVariantType());
    if(ordinalFloat){
        ordinalFloat->setMinValue(parameterVariant.getFloat());
    }else if(ordinalInt){
        ordinalInt->setMinValue(parameterVariant.getInt());
    }else if(ordinalIvec2){
        ordinalIvec2->setMinValue(parameterVariant.getIVec2());
    }else if(ordinalIvec3){
        ordinalIvec3->setMinValue(parameterVariant.getIVec3());
    }else if(ordinalIvec4){
        ordinalIvec4->setMinValue(parameterVariant.getIVec4());
    }else if(ordinalMat2){
        ordinalMat2->setMinValue(parameterVariant.getMat2());
    }else if(ordinalMat3){
        ordinalMat3->setMinValue(parameterVariant.getMat3());
    }else if(ordinalMat4){
        ordinalMat4->setMinValue(parameterVariant.getMat4());
    }else if(ordinalVec2){
        ordinalVec2->setMinValue(parameterVariant.getVec2());
    }else if(ordinalVec3){
        ordinalVec3->setMinValue(parameterVariant.getVec3());
    }else if(ordinalVec4){
        ordinalVec4->setMinValue(parameterVariant.getVec4());
    }else{
        LogErrorCustom("inviwo_setPropertyMinValue","Unknown parameter type" );
    }

    Py_RETURN_NONE;
}

static PyObject* inviwo_listProperties(PyObject* /*self*/, PyObject* args){
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
        auto props = processor->getProperties();
        for(auto p = props.begin(); p != props.end();++p){
            std::string name = (*p)->getIdentifier();
            std::string type  = (*p)->getClassName();
            PyRun_SimpleString(("print \""+ name + " : "+ type + "\"").c_str());
        }

    }
    Py_RETURN_NONE;
}





static PyObject* inviwo_listProcessors(PyObject* /*self*/, PyObject* /*args*/){
    auto processors  = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessors();
    for(auto processor = processors.begin();processor!=processors.end();++processor){
        std::string name = (*processor)->getIdentifier();
        std::string type  = (*processor)->getClassName();
        PyRun_SimpleString(("print \""+ name + " : "+ type + "\"").c_str());
    }
    Py_RETURN_NONE;
}

static PyObject* inviwo_canvas_count(PyObject* /*self*/, PyObject* /*args*/){
    auto canvases = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>();
    return Py_BuildValue("i",canvases.size());
}


static PyObject* inviwo_snapshot(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 1 && PyTuple_Size(args) != 2) {
        std::ostringstream errStr;
        errStr << "snapshot() takes 1 or 2 argument: filename, canvas name";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }
    std::string filename,canvasName = "";

    // check parameter if is string
    if (!PyString_Check(PyTuple_GetItem(args, 0))) {
        PyErr_SetString(PyExc_TypeError, "snapshot() first argument must be a string");
        return 0;
    }

    // check parameter if is string
    if (PyTuple_Size(args) == 2 && !PyString_Check(PyTuple_GetItem(args, 1))) {
        PyErr_SetString(PyExc_TypeError, "snapshot() second argument must be a string");
        return 0;
    }
    filename = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
    if(PyTuple_Size(args) == 2){
        canvasName = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));
    }
    CanvasProcessor *canvas = 0;
    if(canvasName.size()!=0){
        canvas = dynamic_cast<CanvasProcessor*>(InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(canvasName));
    }else{
        auto canvases = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>();
        if(canvases.size()!=0)
            canvas = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>()[0];
    }

    if(!canvas){
        PyErr_SetString(PyExc_TypeError, "snapshot() no canvas found");
        return 0;
    }
    
    canvas->createSnapshot(filename.c_str());
    Py_RETURN_NONE;
}

static PyObject* inviwo_snapshotCanvas(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 2) {
        std::ostringstream errStr;
        errStr << "snapshotCanvas() takes exactly 2 argument: canvas ID, filename";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    // check parameter if is string
    if (!PyInt_Check(PyTuple_GetItem(args, 0))) {
        PyErr_SetString(PyExc_TypeError, "snapshotCanvas() first argument must be an integer");
        return 0;
    }

    // check parameter if is string
    if (!PyString_Check(PyTuple_GetItem(args, 1))) {
        PyErr_SetString(PyExc_TypeError, "snapshotCanvas() second argument must be a string");
        return 0;
    }

    unsigned int index;
    const char* filename = 0;
    if (!PyArg_ParseTuple(args, "is:canvasSnapshot", &index, &filename))
        return 0;
    auto canvases = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>();
    if(index>=canvases.size()){
        std::string msg = std::string("snapshotCanvas() index out of range with index: ") + toString(index) + " ,canvases avilable: " + toString(canvases.size());
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }
    canvases[index]->createSnapshot(filename);
    Py_RETURN_NONE;
}

static PyObject* inviwo_setViewport(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 2) {
        std::ostringstream errStr;
        errStr << "setViewport(width,height) takes exactly 2 argument: width and height";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    int w,h;
    if(!PyArg_ParseTuple(args,"ii",&w,&h)){
        std::ostringstream errStr;
        errStr << "setViewport(width,height) failed to parse input parameters, expects 2 integers, width and height";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }


    if(InviwoApplication::getPtr() && InviwoApplication::getPtr()->getProcessorNetwork()){
        auto canvases = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>();
        for(auto canvas = canvases.begin(); canvas != canvases.end();++canvas){
            static_cast<ProcessorWidgetQt*>((*canvas)->getProcessorWidget())->resize(w,h);
        }
        Py_RETURN_NONE;
    }
    return 0;
    
}


static PyObject* ricen76_test(PyObject* /*self*/, PyObject* args){
    Py_RETURN_NONE;
}

static PyObject* inviwo_redrawOrRenderOrWhatever(PyObject* /*self*/, PyObject* /*args*/){
    auto qtApp = static_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());
    auto mainW = static_cast<InviwoMainWindow*>(qtApp->getMainWindow());
    
    mainW->repaint();
}

static PyMethodDef inviwo_methods[] = {
    {
        "info",
            inviwo_info,
            METH_VARARGS,
            "info() Prints documentation of the module's functions."
    },
    {
        "setPropertyValue",
            inviwo_setPropertyValue,
            METH_VARARGS,
            "setPropertyValue(processor name, property id, scalar or tuple). Assigns a value to a processor property. The value has to be passed as scalar or tuple, depending on the property's cardinality. Camera properties take a 3-tuple of 3-tuples, containing the position, focus and up vectors. Option properties expect an option key."
    },
    {
        "setPropertyMaxValue",
            inviwo_setPropertyMaxValue,
            METH_VARARGS,
            "setPropertyValue(processor name, property id, scalar or tuple). Defines the max value for a property."
    },
    {
        "setPropertyMinValue",
            inviwo_setPropertyMinValue,
            METH_VARARGS,
            "setPropertyMinValue(processor name, property id, scalar or tuple). Defines the min value for a property."
    },
    {
        "listProperties",
            inviwo_listProperties,
            METH_VARARGS,
            "listProperties(processor name). List all properties for a processor."
    },
    {
        "listProcessors",
            inviwo_listProcessors,
            METH_VARARGS,
            "listProcessors(). Lists all processors in the current network."
    },
    {
        "canvasCount",
            inviwo_canvas_count,
            METH_VARARGS,
            "canvasCount(). returns the number of canvases in the current network."
    },
    {
        "snapshot",
            inviwo_snapshot,
            METH_VARARGS,
            "snapshot(filename, [canvas]). Saves a snapshot of the specified canvas to the given file. If no canvas name is passed, the first canvas in the network is chosen."
    },
    {
        "snapshotCanvas",
            inviwo_snapshotCanvas,
            METH_VARARGS,
            "snapshotCanvas(i, filename). Saves a snapshot of the ith canvas to the given file. "
    },
    {
        "setViewport",
            inviwo_setViewport,
            METH_VARARGS,
            "setViewport(width,height). Resizes all the canvases in the network to the given size."
    },
    // {
        // "test",
            // ricen76_test,
            // METH_VARARGS,
            // "test(). This should be removed"
    // },
    { NULL, NULL, 0, NULL} // sentinal
};


// module 'inviwo_internal'
static PyMethodDef internal_methods[] = {
    {
        "ivwPrint",
            inviwo_print,
            METH_VARARGS,
            "Internal helper function used for accessing script output"
    },
    { NULL, NULL, 0, NULL} // sentinal
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
            if(!outputCatcher->load(appQt->getBasePath() +"modules/python/scripts/outputcatcher.py",false)){
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