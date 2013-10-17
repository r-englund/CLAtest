#ifndef IVW_PYCANVASMEHTODSINVIWO_H
#define IVW_PYCANVASMEHTODSINVIWO_H



#include <modules/python/pythonmoduledefine.h>

#include "../pythoninterface/pymethod.h"

namespace inviwo {
PyObject* py_canvascount(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_setViewport(PyObject* /*self*/, PyObject* /*args*/);


    class IVW_MODULE_PYTHON_API PyCanvasCountMethod : public PyMethod{
    public:
        char *getName(){return "canvasCount";}
        char *getDesc(){return "canvasCount()\tReturns the number of canvases in the current network.";}
        virtual PyCFunction getFunc(){return py_canvascount;}
    };

    class IVW_MODULE_PYTHON_API PySetViewportMethod : public PyMethod{
    public:
        char *getName(){return "setViewport";}
        char *getDesc(){return "setViewport(width,height)\tResizes all the canvases in the network to the given size.";}
        virtual PyCFunction getFunc(){return py_setViewport;}
    };

} //namespace


#endif // IVW_PYCANVASMEHTODSINVIWO_H


