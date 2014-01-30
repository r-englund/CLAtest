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

#ifndef IVW_PYMEHTODINVIWO_H
#define IVW_PYMEHTODINVIWO_H

#include <modules/python/pythonmoduledefine.h>


#ifdef _DEBUG
    #undef _DEBUG //Prevent linking debug build of python
    #include <Python.h>
    #define _DEBUG 1
#else
    #include <Python.h>
#endif

#include <vector>
#include <string>


namespace inviwo {
    class PyParamBase{
    public:
        PyParamBase(std::string paramName,bool optional);
        virtual ~PyParamBase(){}
        std::string getParamName()const;
        bool isOptional()const;
        virtual bool testParam(void*) = 0;
        virtual std::string paramType()const = 0;
    private:
        std::string name_;
        bool optional_;
    };

    template<typename T>
    class PyParam : public PyParamBase{
    public:
        PyParam(std::string paramName,bool optional) : PyParamBase(paramName,optional){}
        virtual ~PyParam(){}
        virtual bool testParam(void*){
            return true;//PyValueParser::is<>
        }
    };

#define PY_PARAM(T,t,n) class PyParam##t : public PyParam<T>{ \
public:\
    PyParam##t(std::string paramName,bool optional = false) :PyParam<T>(paramName,optional){} \
    virtual ~PyParam##t(){}\
    virtual std::string paramType()const{return n;}\
    };

    PY_PARAM(std::string,String,"string")
    PY_PARAM(int,Int,"int")
    PY_PARAM(float,Float,"int")
    PY_PARAM(vec2,Vec2,"vec2")
    PY_PARAM(vec3,Vec3,"vec3")
    PY_PARAM(vec4,Vec4,"vec4")

  class IVW_MODULE_PYTHON_API PyMethod{
  public:
      PyMethod();
      virtual ~PyMethod(){}
      virtual std::string getName() = 0;
      virtual std::string getDesc() = 0;
      virtual std::string getParamDesc();
      virtual PyCFunction getFunc() = 0;
      virtual int getFlags(){return METH_VARARGS;};

      PyMethodDef* getDef();


  protected:
      void addParam(PyParamBase* param);
      PyMethodDef def_;
      std::vector<PyParamBase*> params_;

  };
} //namespace

#endif // IVW_PYMEHTODINVIWO_H
