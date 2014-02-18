/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Rickard Englund
 *
 *********************************************************************************/

#ifndef IVW_PYVALUEPARSERINVIWO_H
#define IVW_PYVALUEPARSERINVIWO_H

#include <modules/python/pythonmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/property.h>

#ifdef _DEBUG
#undef _DEBUG //Prevent linking debug build of python
#include <Python.h>
#define _DEBUG 1
#else
#include <Python.h>
#endif

#include <vector>

namespace inviwo {

class IVW_MODULE_PYTHON_API PyValueParser {
    PyValueParser() {}
public:

    template <typename T> static T parse(PyObject* arg);
    template <typename T> static PyObject* toPyObject(T t);
    static void setProperty(Property* p,PyObject* args);
    static PyObject* getProperty(Property* p);

    template<typename T> static bool is(PyObject* arg);

};


template <> bool        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> double      IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> float       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> char        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> short       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> int         IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <>unsigned int IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> long        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> vec2        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> vec3        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> vec4        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> ivec2       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> ivec3       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> ivec4       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> uvec2       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> uvec3       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> uvec4       IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> mat2        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> mat3        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> mat4        IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);
template <> std::string IVW_MODULE_PYTHON_API PyValueParser::parse(PyObject* args);


template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<bool>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<double>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<float>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<char>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<short>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<int>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<unsigned int>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<long>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<vec2>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<vec3>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<vec4>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<ivec2>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<ivec3>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<ivec4>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<uvec2>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<uvec3>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<uvec4>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<mat2>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<mat3>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<mat4>(PyObject* arg);
template <> bool IVW_MODULE_PYTHON_API PyValueParser::is<std::string>(PyObject* arg);


} //namespace

#endif // IVW_PYVALUEPARSERINVIWO_H
