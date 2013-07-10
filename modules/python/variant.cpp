#ifdef IVW_PYTHON
#include "pythonscript.h"

#include <inviwo/core/util/variant.h>

namespace inviwo {

Variant::Variant(PyObject *obj, VariantType type):value_(0),currentType_(type){
    setPython(obj,type);
}

Variant::Variant(PyObject *obj, int type):value_(0),currentType_(VariantType(type)){
    setPython(obj,VariantType(type));
}

void Variant::setPython(PyObject *obj, VariantType type){
    switch (type) {
    case VariantTypeBool:
        {
            bool s = static_cast<bool>(PyInt_AsLong(obj));
            if (PyErr_Occurred()){
                LogError("PyObject did not contain a Variant of type " + type);
            }else
                set<bool>(s, type);
            break;
        }
    case VariantTypeDouble:
        {
            double s = PyFloat_AsDouble(obj);
            if (PyErr_Occurred()){
                LogError("PyObject did not contain a Variant of type " + type);
            }else
                set<double>(s, type);
            break;
        }
    case VariantTypeFloat:
        {
            float s = static_cast<float>(PyFloat_AsDouble(obj));
            if (PyErr_Occurred()){
                LogError("PyObject did not contain a Variant of type " + type);
            }else
                set<float>(s, type);
            break;
        }
    case VariantTypeInteger:
        {
            int s = static_cast<int>(PyInt_AsLong(obj));
            if (PyErr_Occurred()){
                LogError("PyObject did not contain a Variant of type " + type);
            }else
                set<int>(s, type);
            break;
        }
    case VariantTypeLong:
        {
            long s = PyInt_AsLong(obj);
            if (PyErr_Occurred()){
                LogError("PyObject did not contain a Variant of type " + type);
            }else
                set<long>(s, type);
            break;
        }
    case VariantTypeString:
        {
            char* s = PyString_AsString(obj);
            if (PyErr_Occurred()){
                LogError("PyObject did not contain a Variant of type " + type);
            }else
                set<std::string>(std::string(s), type);
            break;
        }
    case VariantTypeIVec2:
        {
            ivec2 v;
            bool success = PyArg_ParseTuple(obj, "ii" , &v.x, &v.y);
            if (success)
                set<ivec2>(v, type);
            else{
                LogError("PyObject did not contain a Variant of type " + type);
            }
            break;
        }
    case VariantTypeIVec3:
        {
            ivec3 v;
            bool success = PyArg_ParseTuple(obj, "iii", &v.x, &v.y, &v.z);
            if (success)
                set<ivec3>(v, type);
            else{
                LogError("PyObject did not contain a Variant of type " + type);
            }
            break;
        }
    case VariantTypeIVec4:
        {
            ivec4 v;
            bool success = PyArg_ParseTuple(obj, "iiii", &v.x, &v.y, &v.z, &v.w);
            if (success)
                set<ivec4>(v, type);
            else{
                LogError("PyObject did not contain a Variant of type " + type);
            }
            break;
        }
    case VariantTypeVec2:
        {
            vec2 v;
            bool success = PyArg_ParseTuple(obj, "fff", &v.x, &v.y);
            if (success)
                set<vec2>(v, type);
            else{
                LogError("PyObject did not contain a Variant of type " + type);
            }
            break;
        }
    case VariantTypeVec3:
        {
            vec3 v;
            bool success = PyArg_ParseTuple(obj, "fff", &v.x, &v.y, &v.z);
            if (success)
                set<vec3>(v, type);
            else{
                LogError("PyObject did not contain a Variant of type " + type);
            }
            break;
        }
    case VariantTypeVec4:
        {
            vec4 v;
            bool success = PyArg_ParseTuple(obj, "ffff", &v.x, &v.y, &v.z, &v.w);
            if (success)
                set<vec4>(v, type);
            else{
                LogError("PyObject did not contain a Variant of type " + type);
            }
            break;
        }
    case VariantTypeDVec2:
        {
            dvec2 v;
            bool success = PyArg_ParseTuple(obj, "dd", &v.x, &v.y);
            if (success)
                set<dvec2>(v, type);
            else{
                LogError("PyObject did not contain a Variant of type " + type);
            }
            break;
        }
    case VariantTypeDVec3:
        {
            dvec3 v;
            bool success = PyArg_ParseTuple(obj, "ddd", &v.x, &v.y, &v.z);
            if (success)
                set<dvec3>(v, type);
            else{
                LogError("PyObject did not contain a Variant of type " + type);
            }
            break;
        }
    case VariantTypeDVec4:
        {
            dvec4 v;
            bool success = PyArg_ParseTuple(obj, "dddd", &v.x, &v.y, &v.z, &v.w);
            if (success)
                set<dvec4>(v, type);
            else{
                LogError("PyObject did not contain a Variant of type " + type);
            }
            break;
        }
    case VariantTypeMat2:
        {
            mat2 m;
            bool success = PyArg_ParseTuple(obj, "(ff)(ff)", &m[0][0], &m[0][1],
                &m[1][0], &m[1][1]);
            if (success)
                set<mat2>(m, type);
            else{
                LogError("PyObject did not contain a Variant of type " + type);
            }
            break;
        }
    case VariantTypeMat3:
        {
            mat3 m;
            bool success = PyArg_ParseTuple(obj, "(fff)(fff)", &m[0][0], &m[0][1], &m[0][2],
                &m[1][0], &m[1][1], &m[1][2],
                &m[2][0], &m[2][1], &m[2][2]);
            if (success)
                set<mat3>(m, type);
            else{
                LogError("PyObject did not contain a Variant of type " + type);
            }
            break;
        }
    case VariantTypeMat4:
        {
            mat4 m;
            bool success = PyArg_ParseTuple(obj, "(ffff)(ffff)", &m[0][0], &m[0][1], &m[0][2], &m[0][3],
                &m[1][0], &m[1][1], &m[1][2], &m[1][3],
                &m[2][0], &m[2][1], &m[2][2], &m[2][3],
                &m[3][0], &m[3][1], &m[3][2], &m[3][3]);
            if (success)
                set<mat4>(m, type);
            else{
                LogError("PyObject did not contain a Variant of type " + type);
            }
            break;
        }
        /*case VariantTypeCamera:
        {
        vec3 position, focus, upVector;
        bool success = PyArg_ParseTuple(obj, "(fff)(fff)(fff)", &position.x, &position.y, &position.z,
        &focus.x, &focus.y, &focus.z,
        &upVector.x, &upVector.y, &upVector.z);
        if (success) {
        Camera* camera = new Camera(position, focus, upVector);
        set<Camera>(*camera, type);
        }
        else{
        LogError("PyObject did not contain a Variant of type " + type);
        }
        break;
        }*/
    /*case VariantTypeTransFunc:
        {
            int numKeys = PyTuple_Size(obj) - 1;
            if (PyTuple_Size(obj) > 0) {
                PyObject* widthObj = PyTuple_GetItem(obj, 0);
                int width = static_cast<int>(PyInt_AsLong(widthObj));
                TransFuncIntensity* transFunc = new TransFuncIntensity(width);
                for (int i = 1; i <= numKeys; ++i) {
                    PyObject* currentObj = PyTuple_GetItem(obj, i);
                    float intensity;
                    int colorLR, colorLG, colorLB, colorLA, colorRR, colorRG, colorRB, colorRA;
                    int isSplit;
                    bool success = PyArg_ParseTuple(currentObj, "fiiiiiiiii", 
                        &intensity,
                        &colorLR, &colorLG, &colorLB, &colorLA,
                        &isSplit,
                        &colorRR, &colorRG, &colorRB, &colorRA);
                    if (success) {
                        col4 colorL(static_cast<uint8_t>(colorLR), static_cast<uint8_t>(colorLG), static_cast<uint8_t>(colorLB), static_cast<uint8_t>(colorLA));
                        TransFuncMappingKey* mappingKey = new TransFuncMappingKey(intensity, colorL);
                        if (static_cast<bool>(isSplit)) {
                            col4 colorR(static_cast<uint8_t>(colorRR), static_cast<uint8_t>(colorRG), static_cast<uint8_t>(colorRB), static_cast<uint8_t>(colorRA));
                            mappingKey->setSplit(true);
                            mappingKey->setColorR(colorR);
                        }
                        transFunc->addKey(mappingKey);

                    }
                    else
                        LogError("Error occurred while parsing PyObject for TransFuncMappingKey");
                }
                set(*transFunc, VariantTypeTransFunc);
            }
            else
                LogError("PyObject did not contain a Variant of type " + type);
            break;
        }*/
    case VariantTypeInvalid:
        LogError("Operation Not Defined For InvalidVariant Exception");
    default:
        LogError("Variant: Conversion from PyObject to " + typeToName(currentType_) + " not implemented");
    }
}


} // namespace

#endif IVW_PYTHON