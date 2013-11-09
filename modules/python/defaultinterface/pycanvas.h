#ifndef IVW_PYCANVASMEHTODSINVIWO_H
#define IVW_PYCANVASMEHTODSINVIWO_H



#include <modules/python/pythonmoduledefine.h>

#include "../pythoninterface/pymethod.h"

namespace inviwo {
PyObject* py_canvascount(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_resizecanvas(PyObject* /*self*/, PyObject* /*args*/);


    class IVW_MODULE_PYTHON_API PyCanvasCountMethod : public PyMethod{
    public:
        char *getName(){return "canvasCount";}
        char *getDesc(){return "canvasCount()\tReturns the number of canvases in the current network.";}
        virtual PyCFunction getFunc(){return py_canvascount;}
    };

	class IVW_MODULE_PYTHON_API PyResizeCanvasMethod : public PyMethod{
	public:
	char *getName(){return "resizeCanvas";}
	char *getDesc(){return "resizeCanvas(canvas,width,height)\tResizes all the canvases in the network to the given size. Canvas can either be given using a canvas index (starting at 0) or a canvas ID string ";}
	virtual PyCFunction getFunc(){return py_resizecanvas;}
	};

} //namespace


#endif // IVW_PYCANVASMEHTODSINVIWO_H


