#ifndef IVW_PYCAMERAMEHTODINVIWO_H
#define IVW_PYCAMERAMEHTODINVIWO_H

#ifndef IVW_PYINVIWO_CPP
    #error This file should only be included from pyinviwo.cpp
#endif

#include <modules/python/pythonmoduledefine.h>

#include "pythonMethod.h"

static PyObject* py_setCameraFocus(PyObject* /*self*/, PyObject* /*args*/);
static PyObject* py_setCameraUp(PyObject* /*self*/, PyObject* /*args*/);
static PyObject* py_setCameraPos(PyObject* /*self*/, PyObject* /*args*/);

namespace inviwo {
    class PySetCameraFocusMethod : public PythonMethod{
    public:
        char *getName(){return "setCameraFocus";}
        char *getDesc(){return "setCameraFocus(processor name, property id, tuple). Function to set the cameras focal point.";}
        virtual PyCFunction getFunc(){return py_setCameraFocus;}

    };

    class PySetCameraUpMethod : public PythonMethod{
    public:
        char *getName(){return "setCameraUp";}
        char *getDesc(){return "setCameraUpDirection(processor name, property id, tuple). Function to set the cameras up direction.";}
        virtual PyCFunction getFunc(){return py_setCameraUp;}

    };

    class PySetCameraPosMethod : public PythonMethod{
    public:
        char *getName(){return "setCameraPosition";}
        char *getDesc(){return "setCameraUpDirection(processor name, property id, tuple). Function to set the cameras position.";}
        virtual PyCFunction getFunc(){return py_setCameraPos;}

    };

} //namespace



static PyObject* py_setCameraFocus(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 3) {
        std::ostringstream errStr;
        errStr << "setCameraFocus() takes exactly 3 arguments: processor name, property id, value";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    // check parameter 1 and 2, if they are strings
    if (!PyString_Check(PyTuple_GetItem(args, 0)) || !PyString_Check(PyTuple_GetItem(args, 1))) {
        PyErr_SetString(PyExc_TypeError, "setCameraFocus() arguments 1 and 2 must be strings");
        return 0;
    }

    std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));
    PyObject* parameter = PyTuple_GetItem(args, 2);

    Processor* processor = getProcessor(processorName);
    if(!processor){
        std::string msg = std::string("setCameraFocus() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("setCameraFocus() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    const std::string className = theProperty->getClassName();
    if(className == "CameraProperty"){
        vec3 focus;
        char *dummy1,*dummy2;
        int d1,d2;
        if(!PyArg_ParseTuple(args,"s#s#(fff)", &dummy1,&d1,&dummy2,&d2,
            &focus.x,&focus.y,&focus.z
            )){
                std::string msg = std::string("setCameraFocus() Failed to parse values for camera, needs to be on the format: (posX,posY,posZ) ") + propertyID;
                PyErr_SetString(PyExc_TypeError, msg.c_str());
                return 0;
        }

        CameraProperty* cam = static_cast<CameraProperty*>(theProperty);
        cam->setLookTo(focus);
        Py_RETURN_NONE;
    }else{
        std::string msg = std::string("setCameraFocus() not a cmera property: ") + className;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }
}

static PyObject* py_setCameraUp(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 3) {
        std::ostringstream errStr;
        errStr << "setCameraUp() takes exactly 3 arguments: processor name, property id, value";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    // check parameter 1 and 2, if they are strings
    if (!PyString_Check(PyTuple_GetItem(args, 0)) || !PyString_Check(PyTuple_GetItem(args, 1))) {
        PyErr_SetString(PyExc_TypeError, "setCameraUp() arguments 1 and 2 must be strings");
        return 0;
    }

    std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));
    PyObject* parameter = PyTuple_GetItem(args, 2);

    Processor* processor = getProcessor(processorName);
    if(!processor){
        std::string msg = std::string("setCameraUp() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("setCameraUp() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    const std::string className = theProperty->getClassName();
    if(className == "CameraProperty"){
        vec3 up;
        char *dummy1,*dummy2;
        int d1,d2;
        if(!PyArg_ParseTuple(args,"s#s#(fff)", &dummy1,&d1,&dummy2,&d2,
            &up.x,&up.y,&up.z
            )){
                std::string msg = std::string("setCameraUp() Failed to parse values for camera, needs to be on the format: (dirX,dirY,dirZ) ") + propertyID;
                PyErr_SetString(PyExc_TypeError, msg.c_str());
                return 0;
        }

        CameraProperty* cam = static_cast<CameraProperty*>(theProperty);
        cam->setLookUp(up);
        Py_RETURN_NONE;
    }else{
        std::string msg = std::string("setCameraUp() not a cmera property: ") + className;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }
}



static PyObject* py_setCameraPos(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 3) {
        std::ostringstream errStr;
        errStr << "setCameraPosition() takes exactly 3 arguments: processor name, property id, value";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    // check parameter 1 and 2, if they are strings
    if (!PyString_Check(PyTuple_GetItem(args, 0)) || !PyString_Check(PyTuple_GetItem(args, 1))) {
        PyErr_SetString(PyExc_TypeError, "setCameraPosition() arguments 1 and 2 must be strings");
        return 0;
    }

    std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));
    PyObject* parameter = PyTuple_GetItem(args, 2);

    Processor* processor = getProcessor(processorName);
    if(!processor){
        std::string msg = std::string("setCameraPosition() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("setCameraPosition() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    const std::string className = theProperty->getClassName();
    if(className == "CameraProperty"){
        vec3 from;
        char *dummy1,*dummy2;
        int d1,d2;
        if(!PyArg_ParseTuple(args,"s#s#(fff)", &dummy1,&d1,&dummy2,&d2,
            &from.x,&from.y,&from.z
            )){
                std::string msg = std::string("setPropertyValue() Failed to parse values for camera, needs to be on the format: (posX,posY,posZ) ") + propertyID;
                PyErr_SetString(PyExc_TypeError, msg.c_str());
                return 0;
        }

        CameraProperty* cam = static_cast<CameraProperty*>(theProperty);
        cam->setLookFrom(from);
        Py_RETURN_NONE;
    }else{
        std::string msg = std::string("setCameraPosition() not a cmera property: ") + className;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }
}



#endif // IVW_PYCAMERAMEHTODINVIWO_H


