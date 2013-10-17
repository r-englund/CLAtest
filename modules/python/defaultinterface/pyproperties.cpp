#include "pyproperties.h"

#include "../pythoninterface/pyvalueparser.h"

#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/core/processors/processor.h>


namespace inviwo {




PyObject* py_setPropertyValue(PyObject* self, PyObject* args){
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

    InviwoApplicationQt* appQt = static_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());  
    Processor* processor = appQt->getProcessorNetwork()->getProcessorByName(processorName);
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

    PyValueParser pyValueParser;
    pyValueParser.setProperty(theProperty,parameter);

    Py_RETURN_NONE;
}





PyObject* py_setPropertyMaxValue(PyObject* /*self*/, PyObject* args){
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

    InviwoApplicationQt* appQt = static_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());  
    Processor* processor = appQt->getProcessorNetwork()->getProcessorByName(processorName);
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

    PyValueParser parser;

    if(ordinalFloat){
        ordinalFloat->setMaxValue(parser.parse<float>(args));
    }else if(ordinalInt){
        ordinalInt->setMaxValue(parser.parse<int>(args));
    }else if(ordinalIvec2){
        ordinalIvec2->setMaxValue(parser.parse<ivec2>(args));
    }else if(ordinalIvec3){
        ordinalIvec3->setMaxValue(parser.parse<ivec3>(args));
    }else if(ordinalIvec4){
        ordinalIvec4->setMaxValue(parser.parse<ivec4>(args));
    }else if(ordinalMat2){
        ordinalMat2->setMaxValue(parser.parse<mat2>(args));
    }else if(ordinalMat3){
        ordinalMat3->setMaxValue(parser.parse<mat3>(args));
    }else if(ordinalMat4){
        ordinalMat4->setMaxValue(parser.parse<mat4>(args));
    }else if(ordinalVec2){
        ordinalVec2->setMaxValue(parser.parse<vec2>(args));
    }else if(ordinalVec3){
        ordinalVec3->setMaxValue(parser.parse<vec3>(args));
    }else if(ordinalVec4){
        ordinalVec4->setMaxValue(parser.parse<vec4>(args));
    }else{
        LogErrorCustom("inviwo_setPropertyMaxValue","Unknown parameter type");
    }

    Py_RETURN_NONE;
}



PyObject* py_setPropertyMinValue(PyObject* /*self*/, PyObject* args){
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

    InviwoApplicationQt* appQt = static_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());  
    Processor* processor = appQt->getProcessorNetwork()->getProcessorByName(processorName);
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

    PyValueParser parser;

    if(ordinalFloat){
        ordinalFloat->setMinValue(parser.parse<float>(args));
    }else if(ordinalInt){
        ordinalInt->setMinValue(parser.parse<int>(args));
    }else if(ordinalIvec2){
        ordinalIvec2->setMinValue(parser.parse<ivec2>(args));
    }else if(ordinalIvec3){
        ordinalIvec3->setMinValue(parser.parse<ivec3>(args));
    }else if(ordinalIvec4){
        ordinalIvec4->setMinValue(parser.parse<ivec4>(args));
    }else if(ordinalMat2){
        ordinalMat2->setMinValue(parser.parse<mat2>(args));
    }else if(ordinalMat3){
        ordinalMat3->setMinValue(parser.parse<mat3>(args));
    }else if(ordinalMat4){
        ordinalMat4->setMinValue(parser.parse<mat4>(args));
    }else if(ordinalVec2){
        ordinalVec2->setMinValue(parser.parse<vec2>(args));
    }else if(ordinalVec3){
        ordinalVec3->setMinValue(parser.parse<vec3>(args));
    }else if(ordinalVec4){
        ordinalVec4->setMinValue(parser.parse<vec4>(args));
    }else{
        LogErrorCustom("inviwo_setPropertyMinValue","Unknown parameter type" );
    }

    Py_RETURN_NONE;
}




PyObject* py_getPropertyValue(PyObject* /*self*/, PyObject* args){
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

    InviwoApplicationQt* appQt = static_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());  
    Processor* processor = appQt->getProcessorNetwork()->getProcessorByName(processorName);
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

    return PyValueParser().getProperty(theProperty);
}

#define CAST_N_GETMAX(PropType,prop,parser) {PropType* casted = dynamic_cast<PropType*>(prop); if(casted) {return parser.toPyObject(casted->getMaxValue());}}
#define CAST_N_GETMIN(PropType,prop,parser) {PropType* casted = dynamic_cast<PropType*>(prop); if(casted) {return parser.toPyObject(casted->getMinValue());}}

PyObject* py_getPropertyMaxValue(PyObject* /*self*/, PyObject* args){
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

    InviwoApplicationQt* appQt = static_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());  
    Processor* processor = appQt->getProcessorNetwork()->getProcessorByName(processorName);
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

    PyValueParser parser;

    CAST_N_GETMAX(FloatProperty,theProperty,parser);
    CAST_N_GETMAX(IntProperty,theProperty,parser);
    CAST_N_GETMAX(IntVec2Property,theProperty,parser);
    CAST_N_GETMAX(IntVec3Property,theProperty,parser);
    CAST_N_GETMAX(IntVec4Property,theProperty,parser);
    CAST_N_GETMAX(FloatMat2Property,theProperty,parser);
    CAST_N_GETMAX(FloatMat3Property,theProperty,parser);
    CAST_N_GETMAX(FloatMat4Property,theProperty,parser);
    CAST_N_GETMAX(FloatVec2Property,theProperty,parser);
    CAST_N_GETMAX(FloatVec3Property,theProperty,parser);
    CAST_N_GETMAX(FloatVec4Property,theProperty,parser);

    LogErrorCustom("inviwo_getPropertyMaxValue","Unknown parameter type");

    Py_RETURN_NONE;
}



PyObject* py_getPropertyMinValue(PyObject* /*self*/, PyObject* args){
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

    InviwoApplicationQt* appQt = static_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());  
    Processor* processor = appQt->getProcessorNetwork()->getProcessorByName(processorName);
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

    PyValueParser parser;

    CAST_N_GETMIN(FloatProperty,theProperty,parser);
    CAST_N_GETMIN(IntProperty,theProperty,parser);
    CAST_N_GETMIN(IntVec2Property,theProperty,parser);
    CAST_N_GETMIN(IntVec3Property,theProperty,parser);
    CAST_N_GETMIN(IntVec4Property,theProperty,parser);
    CAST_N_GETMIN(FloatMat2Property,theProperty,parser);
    CAST_N_GETMIN(FloatMat3Property,theProperty,parser);
    CAST_N_GETMIN(FloatMat4Property,theProperty,parser);
    CAST_N_GETMIN(FloatVec2Property,theProperty,parser);
    CAST_N_GETMIN(FloatVec3Property,theProperty,parser);
    CAST_N_GETMIN(FloatVec4Property,theProperty,parser);

    LogErrorCustom("inviwo_getPropertyMaxValue","Unknown parameter type");

    Py_RETURN_NONE;
}



PyObject* py_clickButton(PyObject* /*self*/, PyObject* args){
    if (PyTuple_Size(args) != 2) {
        std::ostringstream errStr;
        errStr << "clickButton() takes exactly 2 arguments: processor name, property id";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    // check parameter 1 and 2, if they are strings
    if (!PyString_Check(PyTuple_GetItem(args, 0)) || !PyString_Check(PyTuple_GetItem(args, 1))) {
        PyErr_SetString(PyExc_TypeError, "clickButton() arguments 1 and 2 must be strings");
        return 0;
    }

    std::string processorName = std::string(PyString_AsString(PyTuple_GetItem(args, 0)));
    std::string propertyID = std::string(PyString_AsString(PyTuple_GetItem(args, 1)));

    InviwoApplicationQt* appQt = static_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());  
    Processor* processor = appQt->getProcessorNetwork()->getProcessorByName(processorName);
    if(!processor){
        std::string msg = std::string("clickButton() no processor with name: ") + processorName;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    Property *theProperty = processor->getPropertyByIdentifier(propertyID);
    if(!theProperty){
        std::string msg = std::string("clickButton() no property with id: ") + propertyID;
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    ButtonProperty* button = dynamic_cast<ButtonProperty*>(theProperty);
    if(!button){
        std::string msg = std::string("clickButton() no button property with id: ") + propertyID + ", ("+propertyID  +" is of type "+ theProperty->getClassName() +  ")";
        PyErr_SetString(PyExc_TypeError, msg.c_str());
        return 0;
    }

    button->invokeMemberFunctions();

    Py_RETURN_NONE;
}


}