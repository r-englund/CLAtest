/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Rickard Englund
 *
 **********************************************************************/

#include "pyvalueparser.h"

#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/directoryproperty.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/properties/matrixproperties.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <inviwo/core/properties/vectorproperties.h>
#include <inviwo/core/properties/stringproperty.h>

#define PARSETYPE(T,F,S)    T F(PyObject *args){T t;PyArg_ParseTuple(args,S,&t);return t; }
#define PARSEVEC2(T,T2,F,S) T F(PyObject *args){T2 t0,t1;PyArg_ParseTuple(args,S,&t0,&t1);return T(t0,t1); }
#define PARSEVEC3(T,T2,F,S) T F(PyObject *args){T2 t0,t1,t2;PyArg_ParseTuple(args,S,&t0,&t1,&t2);return T(t0,t1,t2); }
#define PARSEVEC4(T,T2,F,S) T F(PyObject *args){T2 t0,t1,t2,t3;PyArg_ParseTuple(args,S,&t0,&t1,&t2,&t3);return T(t0,t1,t2,t3); }

namespace inviwo{
    float parseFloat(PyObject *args){return static_cast<float>(PyFloat_AsDouble(args)); }
    double parseDouble(PyObject *args){return PyFloat_AsDouble(args); }
    PARSETYPE(char,parseChar,"b");
    PARSETYPE(short,parseShort,"h");
    PARSETYPE(int,parseInt,"i");
    PARSETYPE(long,parseLong,"l");

    PARSEVEC2(ivec2,int,parseIVec2,"ii");
    PARSEVEC3(ivec3,int,parseIVec3,"iii");
    PARSEVEC4(ivec4,int,parseIVec4,"iiii");

    PARSEVEC2(uvec2,unsigned int,parseUVec2,"ii");
    PARSEVEC3(uvec3,unsigned int,parseUVec3,"iii");
    PARSEVEC4(uvec4,unsigned int,parseUVec4,"iiii");

    PARSEVEC2(vec2,float,parseVec2,"ff");
    PARSEVEC3(vec3,float,parseVec3,"fff");
    PARSEVEC4(vec4,float,parseVec4,"ffff");

    bool        parseBool(PyObject* args){return PyObject_IsTrue(args) != 0;};
    std::string parseStr(PyObject* args){return std::string(PyString_AsString(args));};
    //std::string parseStr(PyObject* args){char* c;int l;PyArg_ParseTuple(args,"s#",&c,&l);return std::string(c);};

    mat2 parseMat2(PyObject* args){
        mat2 m;
        PyArg_ParseTuple(args,"(ff)(ff)",&m[0][0], &m[0][1], 
                                         &m[1][0], &m[1][1]);
        return m;
    }
    mat3 parseMat3(PyObject* args){
        mat3 m;
        PyArg_ParseTuple(args,"(fff)(fff)(fff)", &m[0][0], &m[0][1], &m[0][2],
                                                 &m[1][0], &m[1][1], &m[1][2],
                                                 &m[2][0], &m[2][1], &m[2][2]);
        return m;
    }
    mat4 parseMat4(PyObject* args){
        mat4 m;
        PyArg_ParseTuple(args,"(ffff)(ffff)(ffff)(ffff)",   &m[0][0], &m[0][1], &m[0][2], &m[0][3],
                                                            &m[1][0], &m[1][1], &m[1][2], &m[1][3],
                                                            &m[2][0], &m[2][1], &m[2][2], &m[2][3],
                                                            &m[3][0], &m[3][1], &m[3][2], &m[3][3]);
        return m;
    }

    template <> bool        PyValueParser::parse(PyObject *args){  return parseBool(args);   }
    template <> double      PyValueParser::parse(PyObject *args){  return parseDouble(args); }
    template <> float       PyValueParser::parse(PyObject *args){  return parseFloat(args);  }
    template <> char        PyValueParser::parse(PyObject *args){  return parseChar(args);   }
    template <> short       PyValueParser::parse(PyObject *args){  return parseShort(args);  }
    template <> int         PyValueParser::parse(PyObject *args){  /*return parseInt(args);*/   return PyInt_AsLong(args); }
    template <> long        PyValueParser::parse(PyObject *args){  return parseLong(args);   }
    template <> vec2        PyValueParser::parse(PyObject *args){  return parseVec2(args);   }
    template <> vec3        PyValueParser::parse(PyObject *args){  return parseVec3(args);   }
    template <> vec4        PyValueParser::parse(PyObject *args){  return parseVec4(args);   }
    template <> ivec2       PyValueParser::parse(PyObject *args){  return parseIVec2(args);  }
    template <> ivec3       PyValueParser::parse(PyObject *args){  return parseIVec3(args);  }
    template <> ivec4       PyValueParser::parse(PyObject *args){  return parseIVec4(args);  }
    template <> uvec2       PyValueParser::parse(PyObject *args){  return parseUVec2(args);  }
    template <> uvec3       PyValueParser::parse(PyObject *args){  return parseUVec3(args);  }
    template <> uvec4       PyValueParser::parse(PyObject *args){  return parseUVec4(args);  }
    template <> mat2        PyValueParser::parse(PyObject *args){  return parseMat2(args);   }
    template <> mat3        PyValueParser::parse(PyObject *args){  return parseMat3(args);   }
    template <> mat4        PyValueParser::parse(PyObject *args){  return parseMat4(args);   }
    template <> std::string PyValueParser::parse(PyObject *args){  return parseStr(args);    }

#if (PY_MAJOR_VERSION == 3  && PY_MINOR_VERSION >= 3) ||  PY_MAJOR_VERSION > 3
    //if python version is 3.3 or larger, use "p", eg parse a real bool obj, else treat it as an int
    template <> PyObject* PyValueParser::toPyObject(bool b){return toPyPy_BuildValueObject("p",b);}
#else
    template <> PyObject* PyValueParser::toPyObject(bool b){return Py_BuildValue("i",b);}
#endif
    
    template <> PyObject* PyValueParser::toPyObject(double d){return Py_BuildValue("d",d);}
    template <> PyObject* PyValueParser::toPyObject(float f){return Py_BuildValue("f",f);}
    template <> PyObject* PyValueParser::toPyObject(char c){return Py_BuildValue("b",c);}
    template <> PyObject* PyValueParser::toPyObject(short s){return Py_BuildValue("s",s);}
    template <> PyObject* PyValueParser::toPyObject(int i){return Py_BuildValue("i",i);}
    template <> PyObject* PyValueParser::toPyObject(long l){return Py_BuildValue("l",l);}
    template <> PyObject* PyValueParser::toPyObject(vec2 v){return Py_BuildValue("ff",v.x,v.y);}
    template <> PyObject* PyValueParser::toPyObject(vec3 v){return Py_BuildValue("fff",v.x,v.y,v.z);}
    template <> PyObject* PyValueParser::toPyObject(vec4 v){return Py_BuildValue("ffff",v.x,v.y,v.z,v.w);}
    template <> PyObject* PyValueParser::toPyObject(ivec2 v){return Py_BuildValue("ii",v.x,v.y);}
    template <> PyObject* PyValueParser::toPyObject(ivec3 v){return Py_BuildValue("iii",v.x,v.y,v.z);}
    template <> PyObject* PyValueParser::toPyObject(ivec4 v){return Py_BuildValue("iiii",v.x,v.y,v.z,v.w);}
    template <> PyObject* PyValueParser::toPyObject(uvec2 v){return Py_BuildValue("ii",v.x,v.y);}
    template <> PyObject* PyValueParser::toPyObject(uvec3 v){return Py_BuildValue("iii",v.x,v.y,v.z);}
    template <> PyObject* PyValueParser::toPyObject(uvec4 v){return Py_BuildValue("iiii",v.x,v.y,v.z,v.w);}
    template <> PyObject* PyValueParser::toPyObject(mat2 m){return Py_BuildValue("(ff)(ff)",m[0][0], m[0][1], 
                                                                                             m[1][0], m[1][1]);}
    template <> PyObject* PyValueParser::toPyObject(mat3 m){return Py_BuildValue("(fff)(fff)(fff)",m[0][0], m[0][1], m[0][2],
                                                                                                    m[1][0], m[1][1], m[1][2],
                                                                                                    m[2][0], m[2][1], m[2][2]);}
    template <> PyObject* PyValueParser::toPyObject(mat4 m){return Py_BuildValue("(ffff)(ffff)(ffff)(ffff)",m[0][0], m[0][1], m[0][2], m[0][3],
                                                                                                             m[1][0], m[1][1], m[1][2], m[1][3],
                                                                                                             m[2][0], m[2][1], m[2][2], m[2][3],
                                                                                                             m[3][0], m[3][1], m[3][2], m[3][3]);}
    template <> PyObject* PyValueParser::toPyObject(std::string str){return Py_BuildValue("s#",str.c_str(),str.length());}

    void PyValueParser::setProperty(Property *p,PyObject *args){
        std::string className = p->getClassName();
        if(className == "BoolProperty")
            static_cast<BoolProperty*>(p)->set(parse<bool>(args));

        else if(className == "FloatProperty")
            static_cast<FloatProperty*>(p)->set(parse<float>(args));

        else if(className == "IntProperty")
            static_cast<IntProperty*>(p)->set(parse<int>(args));

        else if(className == "StringProperty")
            static_cast<StringProperty*>(p)->set(parse<std::string>(args));

        else if(className == "FileProperty")
            static_cast<FileProperty*>(p)->set(parse<std::string>(args));

        else if(className == "IntVec2Property")
            static_cast<IntVec2Property*>(p)->set(parse<ivec2>(args));

        else if(className == "IntVec3Property")
            static_cast<IntVec3Property*>(p)->set(parse<ivec3>(args));

        else if(className == "IntVec4Property")
            static_cast<IntVec4Property*>(p)->set(parse<ivec4>(args));

        else if(className == "FloatVec2Property")
            static_cast<FloatVec2Property*>(p)->set(parse<vec2>(args));

        else if(className == "FloatVec3Property")
            static_cast<FloatVec3Property*>(p)->set(parse<vec3>(args));

        else if(className == "FloatVec4Property")
            static_cast<FloatVec4Property*>(p)->set(parse<vec4>(args));

        else if(className == "FloatMat2Property")
            static_cast<FloatVec2Property*>(p)->set(parse<vec2>(args));

        else if(className == "FloatMat3Property")
            static_cast<FloatVec3Property*>(p)->set(parse<vec3>(args));

        else if(className == "FloatMat4Property")
            static_cast<FloatVec4Property*>(p)->set(parse<vec4>(args));

        else if(className == "FloatMinMaxProperty")
            static_cast<FloatMinMaxProperty*>(p)->set(parse<vec2>(args));


        else if(className == "IntMinMaxProperty")
            static_cast<IntMinMaxProperty*>(p)->set(parse<ivec2>(args));

        else if(className == "CameraProperty"){
            vec3 from,to,up;
            //float fovy,nearP,farP;
            if(!PyArg_ParseTuple(args,"(fff)(fff)(fff)",
                &from.x,&from.y,&from.z,
                &to.x,&to.y,&to.z,
                &up.x,&up.y,&up.z
                //,&fovy,&nearP,&farP
                )){
                    std::string msg = std::string("Failed to parse values for camera, needs to be on the format: ((posX,posY,posZ),(focusX,focusY,focusZ),(upX,upY,upZ)) : ") + p->getIdentifier();
                    PyErr_SetString(PyExc_TypeError, msg.c_str());
                    return;
            }

            CameraProperty* cam = static_cast<CameraProperty*>(p);
            cam->setLook(from, to, up);
        }
        else{
            LogWarn("Unknown Property type : " << className);
            std::string msg = std::string("setPropertyValue() no available conversion for proerty of type: ") + className;
            PyErr_SetString(PyExc_TypeError, msg.c_str());
        }
    }


#define CAST_DO_STUFF(PropType,prop) {PropType* casted = dynamic_cast<PropType*>(prop); if(casted) {return toPyObject(casted->get());}}

    PyObject* PyValueParser::getProperty(Property *p){
        CAST_DO_STUFF(BoolProperty,p);
        CAST_DO_STUFF(FloatProperty,p);
        CAST_DO_STUFF(IntProperty,p);
        CAST_DO_STUFF(StringProperty,p);
        CAST_DO_STUFF(FileProperty,p);
        CAST_DO_STUFF(IntVec2Property,p);
        CAST_DO_STUFF(IntVec3Property,p);
        CAST_DO_STUFF(IntVec4Property,p);
        CAST_DO_STUFF(FloatVec2Property,p);
        CAST_DO_STUFF(FloatVec3Property,p);
        CAST_DO_STUFF(FloatVec4Property,p);
        CAST_DO_STUFF(FloatMat2Property,p);
        CAST_DO_STUFF(FloatMat3Property,p);
        CAST_DO_STUFF(FloatMat4Property,p);
        CAST_DO_STUFF(FloatMinMaxProperty,p);
        CAST_DO_STUFF(IntMinMaxProperty,p);
        
        return 0;
    }

}//namespace
