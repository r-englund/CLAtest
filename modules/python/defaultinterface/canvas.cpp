#include "canvas.h"

#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/qt/widgets/processors/processorwidgetqt.h>
#include <modules/opengl/canvasprocessorgl.h>


namespace inviwo {



PyObject* py_canvascount(PyObject* /*self*/, PyObject* /*args*/){
    if(InviwoApplication::getPtr() && InviwoApplication::getPtr()->getProcessorNetwork()){
        std::vector<CanvasProcessor*> canvases = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>();
        return Py_BuildValue("i",canvases.size());
    }
    Py_RETURN_NONE;

}



PyObject* py_setViewport(PyObject* /*self*/, PyObject* args){
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
        std::vector<CanvasProcessor*> canvases = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>();
        for(std::vector<CanvasProcessor*>::const_iterator canvas = canvases.begin(); canvas != canvases.end();++canvas){
            static_cast<ProcessorWidgetQt*>((*canvas)->getProcessorWidget())->resize(w,h);
        }
        Py_RETURN_NONE;
    }
    return 0;

}

}