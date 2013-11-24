#include "pycanvas.h"

#include <inviwo/core/common/inviwoapplication.h>
//#include <inviwo/qt/widgets/processors/processorwidgetqt.h>
#include <modules/opengl/canvasprocessorgl.h>
#include "../pythoninterface/pyvalueparser.h"


namespace inviwo {



PyObject* py_canvascount(PyObject* /*self*/, PyObject* /*args*/){
    if(InviwoApplication::getPtr() && InviwoApplication::getPtr()->getProcessorNetwork()){
        std::vector<CanvasProcessor*> canvases = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>();
        return Py_BuildValue("i",canvases.size());
    }
    Py_RETURN_NONE;

}


//
PyObject* py_resizecanvas(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 3) {
        std::ostringstream errStr;
        errStr << "resizeCanvas(canvas,width,height) takes exactly 3 argument: canvas,width and height";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

	PyValueParser parser;
	CanvasProcessor* canvas = 0;

	PyObject *arg0 = PyTuple_GetItem(args,0);
	bool argIsString = PyString_Check(arg0);

	if(argIsString){
		std::string id = parser.parse<std::string>(arg0);
		Processor* processor = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(id);
		if(!processor){
			std::string msg = std::string("resizeCanvas(canvas,width,height) no processor with name: ") + id;
			PyErr_SetString(PyExc_TypeError, msg.c_str());
			return 0;
		}

		canvas = dynamic_cast<CanvasProcessor*>(processor);
		if(!canvas){
			std::string msg = std::string("resizeCanvas(canvas,width,height) processor with name: ") + id + " is not a canvas processor, it is a" + processor->getClassName();
			PyErr_SetString(PyExc_TypeError, msg.c_str());
			return 0;
		}
	}else{
		int id = parser.parse<int>(arg0);
		std::vector<CanvasProcessor*> canvases = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorsByType<CanvasProcessor>();
		if(canvases.size()==0){
			std::string msg = std::string("resizeCanvas(canvas,width,height) no canvases found in current network") ;
			PyErr_SetString(PyExc_TypeError, msg.c_str());
			return 0;
		}

		if(static_cast<int>(canvases.size())<=id){
			std::string msg = std::string("resizeCanvas(canvas,width,height) index out of bounds, index given: ") + toString(id) + ", max index possible: " + toString(canvases.size()-1) ;
			PyErr_SetString(PyExc_TypeError, msg.c_str());
			return 0;
		}

		canvas = canvases[id];
	}


	PyObject *arg1 = PyTuple_GetItem(args,1);
	PyObject *arg2 = PyTuple_GetItem(args,2);

	int w = parser.parse<int>(arg1);
	int h = parser.parse<int>(arg1);

	if(w <= 0 || h <= 0){
		std::string msg = std::string("resizeCanvas(canvas,width,height) width and height must have positive non-zero values ");
		PyErr_SetString(PyExc_TypeError, msg.c_str());
		return 0;
	}

	static_cast<IntVec2Property*>(canvas->getPropertyByIdentifier("dimensions"))->set(ivec2(w,h));
	canvas->invalidate(PropertyOwner::INVALID_OUTPUT);
	Py_RETURN_NONE;

}

}