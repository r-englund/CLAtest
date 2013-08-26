#include "snapshot.h"


#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/core/processors/processor.h>
#include <modules/opengl/canvasprocessorgl.h>



namespace inviwo {

PyObject* py_snapshot(PyObject* /*self*/, PyObject* args){
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
        if(InviwoApplication::getPtr() && InviwoApplication::getPtr()->getProcessorNetwork()){
            std::vector<CanvasProcessor*> canvases = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>();
            if(canvases.size()!=0)
                canvas = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>()[0];
        }
    }

    if(!canvas){
        PyErr_SetString(PyExc_TypeError, "snapshot() no canvas found");
        return 0;
    }

    canvas->createSnapshot(filename.c_str());
    Py_RETURN_NONE;
}



PyObject* py_snapshotCanvas(PyObject* /*self*/, PyObject* args){
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

    std::vector<CanvasProcessor*> canvases = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>();
    if(index>=canvases.size()){
        std::string msg = std::string("snapshotCanvas() index out of range with index: ") + toString(index) + " ,canvases avilable: " + toString(canvases.size());
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }
    canvases[index]->createSnapshot(filename);
    Py_RETURN_NONE;
}

}