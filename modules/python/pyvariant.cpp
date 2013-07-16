#include "pythonscript.h"

#include "pyvariant.h"

namespace inviwo {

PyVariant::PyVariant(PyObject *obj, VariantType type):Variant(type){
    setPython(obj,type);
}

PyVariant::PyVariant(PyObject *obj, int type):Variant(VariantType(type)){
    setPython(obj,VariantType(type));
}

PyVariant::PyVariant(const Variant& obj):Variant(obj){
}

PyVariant::PyVariant(const VariantType& type):Variant(type){
}

PyVariant::PyVariant(const int& type):Variant(VariantType(type)){
}

void PyVariant::setPython(PyObject *obj, VariantType type){
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
    case VariantTypeInvalid:
        LogError("Operation Not Defined For InvalidVariant Exception");
    default:
        LogError("Variant: Conversion from PyObject to " + typeToName(currentType_) + " not implemented");
    }
}

PyObject *PyVariant::getAsPythonObject(){
    switch(currentType_){
    case Variant::VariantTypeInvalid:
        LogError("VariantTypeInvalid type not supported");
        return 0;
    case Variant::VariantTypeBool:
        return Py_BuildValue("i",getBool());
    case Variant::VariantTypeDouble:
        return Py_BuildValue("d",getDouble());
    case Variant::VariantTypeFloat:
        return Py_BuildValue("f",getFloat());
    case Variant::VariantTypeInteger:
        return Py_BuildValue("i",getInt());
    case Variant::VariantTypeLong:
        return Py_BuildValue("l",getLong());
    case Variant::VariantTypeString:
        {
            const std::string s = getString();
            return Py_BuildValue("s",s.c_str(),s.length());
        }


    case Variant::VariantTypeIVec2:
        {
            const ivec2 &p = getIVec2();
            return Py_BuildValue("(ii)",p.x,p.y);
        }
    case Variant::VariantTypeIVec3:
        {
            const ivec3 &p = getIVec3();
            return Py_BuildValue("(iii)",p.x,p.y,p.z);
        }
    case Variant::VariantTypeIVec4:
        {
            const ivec4 &p = getIVec4();
            return Py_BuildValue("(iiii)",p.x,p.y,p.z,p.w);
        }


    case Variant::VariantTypeVec2:
        {
            const vec2 &p = getVec2();
            return Py_BuildValue("(ff)",p.x,p.y);
        }
    case Variant::VariantTypeVec3:
        {
            const vec3 &p = getVec3();
            return Py_BuildValue("(fff)",p.x,p.y,p.z);
        }
    case Variant::VariantTypeVec4:
        {
            const vec4 &p = getVec4();
            return Py_BuildValue("(ffff)",p.x,p.y,p.z,p.w);
        }



    case Variant::VariantTypeDVec2:
        {
            const dvec2 &p = getDVec2();
            return Py_BuildValue("(dd)",p.x,p.y);
        }
    case Variant::VariantTypeDVec3:
        {
            const dvec3 &p = getDVec3();
            return Py_BuildValue("(ddd)",p.x,p.y,p.z);
        }
    case Variant::VariantTypeDVec4:
        {
            const dvec4 &p = getDVec4(); 
            return Py_BuildValue(" (dddd)",p.x,p.y,p.z,p.w);
        }



    case Variant::VariantTypeMat2:
        {
            const mat2 &m = getMat2();
            return Py_BuildValue("((ff)(ff))",m[0][0],m[0][1],
                                              m[1][0],m[1][1]);
        }
    case Variant::VariantTypeMat3:
        {
            const mat3 &m = getMat3();
            return Py_BuildValue("((fff)(fff)(fff))",m[0][0],m[0][1],m[0][2],
                                                     m[1][0],m[1][1],m[1][2],
                                                     m[2][0],m[2][1],m[2][2]);
        }
    case Variant::VariantTypeMat4:
        {
            const mat4 &m = getMat4();
            return Py_BuildValue("((ffff)(ffff)(ffff)(ffff))",m[0][0],m[0][1],m[0][2],m[0][3],
                                                        m[1][0],m[1][1],m[1][2],m[1][3],
                                                        m[2][0],m[2][1],m[2][2],m[2][3],
                                                        m[3][0],m[3][1],m[3][2],m[3][3]);
        }
    default:
        LogWarn("Variant type not yet implemented");
        Py_RETURN_NONE;
    }
    LogError("Should not reach here");

}


} // namespace