#ifndef IVW_PROPERTIESMEHTODSINVIWO_H
#define IVW_PROPERTIESMEHTODSINVIWO_H

#ifndef IVW_PYINVIWO_CPP
    #error This file should only be included from pyinviwo.cpp
#endif

#include <modules/python/pythonmoduledefine.h>

#include "pythonMethod.h"



static PyObject* py_setPropertyValue(PyObject* /*self*/, PyObject* /*args*/);
static PyObject* py_setPropertyMaxValue(PyObject* /*self*/, PyObject* /*args*/);
static PyObject* py_setPropertyMinValue(PyObject* /*self*/, PyObject* /*args*/);
static PyObject* py_getPropertyValue(PyObject* /*self*/, PyObject* /*args*/);
static PyObject* py_getPropertyMaxValue(PyObject* /*self*/, PyObject* /*args*/);
static PyObject* py_getPropertyMinValue(PyObject* /*self*/, PyObject* /*args*/);

namespace inviwo {
    class PySetPropertyValueMethod : public PythonMethod{
    public:
        char *getName(){return "setPropertyValue";}
        char *getDesc(){return "setPropertyValue(processor name, property id, scalar or tuple). Assigns a value to a processor property. The value has to be passed as scalar or tuple, depending on the property's cardinality. Camera properties take a 3-tuple of 3-tuples, containing the position, focus and up vectors. Option properties expect an option key.";}
        virtual PyCFunction getFunc(){return py_setPropertyValue;}
    };

    class PySetPropertyMaxValueMethod : public PythonMethod{
    public:
        char *getName(){return "setPropertyMaxValue";}
        char *getDesc(){return "setPropertyValue(processor name, property id, scalar or tuple). Defines the max value for a property.";}
        virtual PyCFunction getFunc(){return py_setPropertyMaxValue;}
    };

    class PySetPropertyMinValueMethod : public PythonMethod{
    public:
        char *getName(){return "setPropertyMinValue";}
        char *getDesc(){return "setPropertyMinValue(processor name, property id, scalar or tuple). Defines the min value for a property.";}
        virtual PyCFunction getFunc(){return py_setPropertyMinValue;}
    };

    class PyGetPropertyValueMethod : public PythonMethod{
    public:
        char *getName(){return "getPropertyValue";}
        char *getDesc(){return "getPropertyValue(processor name, property id).Returns the current value of a processor property (scalar or tuple).";}
        virtual PyCFunction getFunc(){return py_getPropertyValue;}
    };

    class PyGetPropertyMaxValueMethod : public PythonMethod{
    public:
        char *getName(){return "getPropertyMaxValue";}
        char *getDesc(){return "getPropertyValue(processor name, property id). Returns the max value for a property (scalar or tuple).";}
        virtual PyCFunction getFunc(){return py_getPropertyMaxValue;}
    };

    class PyGetPropertyMinValueMethod : public PythonMethod{
    public:
        char *getName(){return "getPropertyMinValue";}
        char *getDesc(){return "getPropertyValue(processor name, property id). Returns the min value for a property (scalar or tuple).";}
        virtual PyCFunction getFunc(){return py_getPropertyMinValue;}
    };

} //namespace




static PyObject* py_setPropertyValue(PyObject* /*self*/, PyObject* args){
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
    if(className == "CameraProperty"){
        vec3 from,to,up;
        //float fovy,nearP,farP;
        char *dummy1,*dummy2;
        int d1,d2;
        if(!PyArg_ParseTuple(args,"s#s#((fff)(fff)(fff))", &dummy1,&d1,&dummy2,&d2,
            &from.x,&from.y,&from.z,
            &to.x,&to.y,&to.z,
            &up.x,&up.y,&up.z
            //,&fovy,&nearP,&farP
            )){
                std::string msg = std::string("setPropertyValue() Failed to parse values for camera, needs to be on the format: ((posX,posY,posZ),(focusX,focusy,focusZ),(upX,upY,upZ)) ") + propertyID;
                PyErr_SetString(PyExc_TypeError, msg.c_str());
                return 0;
        }

        CameraProperty* cam = static_cast<CameraProperty*>(theProperty);
        cam->setLookFrom(from);
        cam->setLookTo(to);
        cam->setLookUp(up);

    }else{
        PyVariant parameterVariant(parameter, theProperty->getVariantType());
        theProperty->setVariant(parameterVariant);        
    }


    Py_RETURN_NONE;
}





static PyObject* py_setPropertyMaxValue(PyObject* /*self*/, PyObject* args){
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

    OrdinalProperty<float>* ordinalFloat = dynamic_cast<OrdinalProperty<float>*>(theProperty);
    OrdinalProperty<int>*   ordinalInt   = dynamic_cast<OrdinalProperty<int>  *>(theProperty);
    OrdinalProperty<ivec2>* ordinalIvec2 = dynamic_cast<OrdinalProperty<ivec2>*>(theProperty);
    OrdinalProperty<ivec3>* ordinalIvec3 = dynamic_cast<OrdinalProperty<ivec3>*>(theProperty);
    OrdinalProperty<ivec4>* ordinalIvec4 = dynamic_cast<OrdinalProperty<ivec4>*>(theProperty);
    OrdinalProperty<mat2>*  ordinalMat2  = dynamic_cast<OrdinalProperty<mat2> *>(theProperty);
    OrdinalProperty<mat3>*  ordinalMat3  = dynamic_cast<OrdinalProperty<mat3> *>(theProperty);
    OrdinalProperty<mat4>*  ordinalMat4  = dynamic_cast<OrdinalProperty<mat4> *>(theProperty);
    OrdinalProperty<vec2>*  ordinalVec2  = dynamic_cast<OrdinalProperty<vec2> *>(theProperty);
    OrdinalProperty<vec3>*  ordinalVec3  = dynamic_cast<OrdinalProperty<vec3> *>(theProperty);
    OrdinalProperty<vec4>*  ordinalVec4  = dynamic_cast<OrdinalProperty<vec4> *>(theProperty);

    PyVariant parameterVariant(parameter, theProperty->getVariantType());
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



static PyObject* py_setPropertyMinValue(PyObject* /*self*/, PyObject* args){
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

    OrdinalProperty<float>* ordinalFloat = dynamic_cast<OrdinalProperty<float>*>(theProperty);
    OrdinalProperty<int>*   ordinalInt   = dynamic_cast<OrdinalProperty<int>  *>(theProperty);
    OrdinalProperty<ivec2>* ordinalIvec2 = dynamic_cast<OrdinalProperty<ivec2>*>(theProperty);
    OrdinalProperty<ivec3>* ordinalIvec3 = dynamic_cast<OrdinalProperty<ivec3>*>(theProperty);
    OrdinalProperty<ivec4>* ordinalIvec4 = dynamic_cast<OrdinalProperty<ivec4>*>(theProperty);
    OrdinalProperty<mat2>*  ordinalMat2  = dynamic_cast<OrdinalProperty<mat2> *>(theProperty);
    OrdinalProperty<mat3>*  ordinalMat3  = dynamic_cast<OrdinalProperty<mat3> *>(theProperty);
    OrdinalProperty<mat4>*  ordinalMat4  = dynamic_cast<OrdinalProperty<mat4> *>(theProperty);
    OrdinalProperty<vec2>*  ordinalVec2  = dynamic_cast<OrdinalProperty<vec2> *>(theProperty);
    OrdinalProperty<vec3>*  ordinalVec3  = dynamic_cast<OrdinalProperty<vec3> *>(theProperty);
    OrdinalProperty<vec4>*  ordinalVec4  = dynamic_cast<OrdinalProperty<vec4> *>(theProperty);

    PyVariant parameterVariant(parameter, theProperty->getVariantType());
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




static PyObject* py_getPropertyValue(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 2) {
        std::ostringstream errStr;
        errStr << "getPropertyValue() takes exactly 2 arguments: processor name, property id";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    // check parameter 1 and 2, if they are strings
    if (!PyString_Check(PyTuple_GetItem(args, 0)) || !PyString_Check(PyTuple_GetItem(args, 1))) {
        PyErr_SetString(PyExc_TypeError, "getPropertyValue() arguments 1 and 2 must be strings");
        return 0;
    }

    std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));

    Processor* processor = getProcessor(processorName);
    if(!processor){
        std::string msg = std::string("getPropertyValue() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("getPropertyValue() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    CameraProperty *cam = dynamic_cast<CameraProperty*>(theProperty);
    if(cam){
        const vec3 &eye = cam->getLookFrom();
        const vec3 &focus = cam->getLookTo();
        const vec3 &up = cam->getLookUp();
        return Py_BuildValue("((fff)(fff)(fff))",eye.x,eye.y,eye.z,focus.x,focus.y,focus.z,up.x,up.y,up.z);
    }else{

        try{
            PyVariant variant(theProperty->getVariant());
            if(variant.isValid())
                return variant.getAsPythonObject();
            else{
                LogWarnCustom("inviwo_getPropertyValue", "getPropertyValue() reading from property of unsupported type: " << theProperty->getClassName());
                Py_RETURN_NONE;
            }
        }catch (...) {
            LogErrorCustom("inviwo_getPropertyValue","Could not convert property to a variant");
            return 0;
        }
    }
    Py_RETURN_NONE;
}

static PyObject* py_getPropertyMaxValue(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 2) {
        std::ostringstream errStr;
        errStr << "getPropertyMaxValue() takes exactly 2 arguments: processor name, property id";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    // check parameter 1 and 2, if they are strings
    if (!PyString_Check(PyTuple_GetItem(args, 0)) || !PyString_Check(PyTuple_GetItem(args, 1))) {
        PyErr_SetString(PyExc_TypeError, "getPropertyMaxValue() arguments 1 and 2 must be strings");
        return 0;
    }

    std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));

    Processor* processor = getProcessor(processorName);
    if(!processor){
        std::string msg = std::string("getPropertyMaxValue() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("getPropertyMaxValue() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    OrdinalProperty<float>* ordinalFloat = dynamic_cast<OrdinalProperty<float>*>(theProperty);
    OrdinalProperty<int>*   ordinalInt   = dynamic_cast<OrdinalProperty<int>  *>(theProperty);
    OrdinalProperty<ivec2>* ordinalIvec2 = dynamic_cast<OrdinalProperty<ivec2>*>(theProperty);
    OrdinalProperty<ivec3>* ordinalIvec3 = dynamic_cast<OrdinalProperty<ivec3>*>(theProperty);
    OrdinalProperty<ivec4>* ordinalIvec4 = dynamic_cast<OrdinalProperty<ivec4>*>(theProperty);
    OrdinalProperty<mat2>*  ordinalMat2  = dynamic_cast<OrdinalProperty<mat2> *>(theProperty);
    OrdinalProperty<mat3>*  ordinalMat3  = dynamic_cast<OrdinalProperty<mat3> *>(theProperty);
    OrdinalProperty<mat4>*  ordinalMat4  = dynamic_cast<OrdinalProperty<mat4> *>(theProperty);
    OrdinalProperty<vec2>*  ordinalVec2  = dynamic_cast<OrdinalProperty<vec2> *>(theProperty);
    OrdinalProperty<vec3>*  ordinalVec3  = dynamic_cast<OrdinalProperty<vec3> *>(theProperty);
    OrdinalProperty<vec4>*  ordinalVec4  = dynamic_cast<OrdinalProperty<vec4> *>(theProperty);

    if(ordinalFloat){
        PyVariant v(ordinalFloat->getVariantType());
        v.setFloat(ordinalFloat->getMaxValue());
        return v.getAsPythonObject();
    }else if(ordinalInt){
        PyVariant v(ordinalInt->getVariantType());
        v.setInt(ordinalInt->getMaxValue());
        return v.getAsPythonObject();
    }else if(ordinalIvec2){
        PyVariant v(ordinalIvec2->getVariantType());
        v.setIVec2(ordinalIvec2->getMaxValue());
        return v.getAsPythonObject();
    }else if(ordinalIvec3){
        PyVariant v(ordinalIvec3->getVariantType());
        v.setIVec3(ordinalIvec3->getMaxValue());
        return v.getAsPythonObject();
    }else if(ordinalIvec4){
        PyVariant v(ordinalIvec4->getVariantType());
        v.setIVec4(ordinalIvec4->getMaxValue());
        return v.getAsPythonObject();
    }else if(ordinalMat2){
        PyVariant v(ordinalMat2->getVariantType());
        v.setMat2(ordinalMat2->getMaxValue());
        return v.getAsPythonObject();
    }else if(ordinalMat3){
        PyVariant v(ordinalMat3->getVariantType());
        v.setMat3(ordinalMat3->getMaxValue());
        return v.getAsPythonObject();
    }else if(ordinalMat4){
        PyVariant v(ordinalMat4->getVariantType());
        v.setMat4(ordinalMat4->getMaxValue());
        return v.getAsPythonObject();
    }else if(ordinalVec2){
        PyVariant v(ordinalVec2->getVariantType());
        v.setVec2(ordinalVec2->getMaxValue());
        return v.getAsPythonObject();
    }else if(ordinalVec3){
        PyVariant v(ordinalVec3->getVariantType());
        v.setVec3(ordinalVec3->getMaxValue());
        return v.getAsPythonObject();
    }else if(ordinalVec4){
        PyVariant v(ordinalVec4->getVariantType());
        v.setVec4(ordinalVec4->getMaxValue());
        return v.getAsPythonObject();
    }else{
        LogErrorCustom("inviwo_getPropertyMaxValue","Unknown parameter type");
    }

    Py_RETURN_NONE;
}



static PyObject* py_getPropertyMinValue(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 2) {
        std::ostringstream errStr;
        errStr << "getPropertyMinValue() takes exactly 2 arguments: processor name, property id";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    // check parameter 1 and 2, if they are strings
    if (!PyString_Check(PyTuple_GetItem(args, 0)) || !PyString_Check(PyTuple_GetItem(args, 1))) {
        PyErr_SetString(PyExc_TypeError, "getPropertyMinValue() arguments 1 and 2 must be strings");
        return 0;
    }

    std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));

    Processor* processor = getProcessor(processorName);
    if(!processor){
        std::string msg = std::string("getPropertyMinValue() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("getPropertyMinValue() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    OrdinalProperty<float>* ordinalFloat = dynamic_cast<OrdinalProperty<float>*>(theProperty);
    OrdinalProperty<int>*   ordinalInt   = dynamic_cast<OrdinalProperty<int>  *>(theProperty);
    OrdinalProperty<ivec2>* ordinalIvec2 = dynamic_cast<OrdinalProperty<ivec2>*>(theProperty);
    OrdinalProperty<ivec3>* ordinalIvec3 = dynamic_cast<OrdinalProperty<ivec3>*>(theProperty);
    OrdinalProperty<ivec4>* ordinalIvec4 = dynamic_cast<OrdinalProperty<ivec4>*>(theProperty);
    OrdinalProperty<mat2>*  ordinalMat2  = dynamic_cast<OrdinalProperty<mat2> *>(theProperty);
    OrdinalProperty<mat3>*  ordinalMat3  = dynamic_cast<OrdinalProperty<mat3> *>(theProperty);
    OrdinalProperty<mat4>*  ordinalMat4  = dynamic_cast<OrdinalProperty<mat4> *>(theProperty);
    OrdinalProperty<vec2>*  ordinalVec2  = dynamic_cast<OrdinalProperty<vec2> *>(theProperty);
    OrdinalProperty<vec3>*  ordinalVec3  = dynamic_cast<OrdinalProperty<vec3> *>(theProperty);
    OrdinalProperty<vec4>*  ordinalVec4  = dynamic_cast<OrdinalProperty<vec4> *>(theProperty);

    if(ordinalFloat){
        PyVariant v(ordinalFloat->getVariantType());
        v.setFloat(ordinalFloat->getMinValue());
        return v.getAsPythonObject();
    }else if(ordinalInt){
        PyVariant v(ordinalInt->getVariantType());
        v.setInt(ordinalInt->getMinValue());
        return v.getAsPythonObject();
    }else if(ordinalIvec2){
        PyVariant v(ordinalIvec2->getVariantType());
        v.setIVec2(ordinalIvec2->getMinValue());
        return v.getAsPythonObject();
    }else if(ordinalIvec3){
        PyVariant v(ordinalIvec3->getVariantType());
        v.setIVec3(ordinalIvec3->getMinValue());
        return v.getAsPythonObject();
    }else if(ordinalIvec4){
        PyVariant v(ordinalIvec4->getVariantType());
        v.setIVec4(ordinalIvec4->getMinValue());
        return v.getAsPythonObject();
    }else if(ordinalMat2){
        PyVariant v(ordinalMat2->getVariantType());
        v.setMat2(ordinalMat2->getMinValue());
        return v.getAsPythonObject();
    }else if(ordinalMat3){
        PyVariant v(ordinalMat3->getVariantType());
        v.setMat3(ordinalMat3->getMinValue());
        return v.getAsPythonObject();
    }else if(ordinalMat4){
        PyVariant v(ordinalMat4->getVariantType());
        v.setMat4(ordinalMat4->getMinValue());
        return v.getAsPythonObject();
    }else if(ordinalVec2){
        PyVariant v(ordinalVec2->getVariantType());
        v.setVec2(ordinalVec2->getMinValue());
        return v.getAsPythonObject();
    }else if(ordinalVec3){
        PyVariant v(ordinalVec3->getVariantType());
        v.setVec3(ordinalVec3->getMinValue());
        return v.getAsPythonObject();
    }else if(ordinalVec4){
        PyVariant v(ordinalVec4->getVariantType());
        v.setVec4(ordinalVec4->getMinValue());
        return v.getAsPythonObject();
    }else{
        LogErrorCustom("inviwo_getPropertyMinValue","Unknown parameter type");
    }

    Py_RETURN_NONE;
}



#endif // IVW_PROPERTIESMEHTODSINVIWO_H


