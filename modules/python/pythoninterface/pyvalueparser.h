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

#ifndef IVW_PYVALUEPARSERINVIWO_H
#define IVW_PYVALUEPARSERINVIWO_H

#include <modules/python/pythonmoduledefine.h>


#ifdef _DEBUG
#undef _DEBUG //Prevent linking debug build of python
#include <Python.h>
#define _DEBUG 1
#else
#include <Python.h>
#endif

#include <vector>

namespace inviwo {

    class IVW_MODULE_PYTHON_API PyValueParser{
    
    public:
        PyValueParser(){}

        template <typename T> T parse(PyObject *arg);
        template <typename T> PyObject* toPyObject(T t);
        void setProperty(Property *p,PyObject *args);
        PyObject *getProperty(Property *p);

    };


    template <> bool        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> double      IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> float       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> char        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> short       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> int         IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> long        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> vec2        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> vec3        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> vec4        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> ivec2       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> ivec3       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> ivec4       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> uvec2       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> uvec3       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> uvec4       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> mat2        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> mat3        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> mat4        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);
    template <> std::string IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject *args);


} //namespace

#endif // IVW_PYVALUEPARSERINVIWO_H
