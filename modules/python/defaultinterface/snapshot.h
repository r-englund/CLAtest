#ifndef IVW_PYSNAPSHOTMEHTODINVIWO_H
#define IVW_PYSNAPSHOTMEHTODINVIWO_H



#include <modules/python/pythonmoduledefine.h>

#include "../pythoninterface/pymethod.h"

namespace inviwo {

PyObject* py_snapshot(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_snapshotCanvas(PyObject* /*self*/, PyObject* /*args*/);

class IVW_MODULE_PYTHON_API PySnapshotMethod : public PyMethod{
    public:
        char *getName(){return "snapshot";}
        char *getDesc(){return "snapshot(filename, [canvas])\tSaves a snapshot of the specified canvas to the given file. If no canvas name is passed, the first canvas in the network is chosen.";}
        virtual PyCFunction getFunc(){return py_snapshot;}
    };

    class IVW_MODULE_PYTHON_API PySnapshotCanvasMethod : public PyMethod{
    public:
        char *getName(){return "snapshotCanvas";}
        char *getDesc(){return "snapshotCanvas(i, filename)\tSaves a snapshot of the ith canvas to the given file.";}
        virtual PyCFunction getFunc(){return py_snapshotCanvas;}
    };

} //namespace



#endif // IVW_PYSNAPSHOTMEHTODINVIWO_H


