/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 * Contact: Sathish Kottravel
 *
 *********************************************************************************/

#ifndef IVW_PYPACKAGEPARSER_H
#define IVW_PYPACKAGEPARSER_H

#include <modules/pypackages/pypackagesmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/python/pythoninterface/pyvalueparser.h>


#ifdef _DEBUG
#undef _DEBUG //Prevent linking debug build of python
#include <Python.h>
#define _DEBUG 1
#else
#include <Python.h>
#endif

#include <vector>

namespace inviwo {

//Single Component
#define TOPYOBJ_1D_DECLARATION(i) template <> PyObject* PyPackageParser::toPyObject<Data##i>(void *, size_t);
#define TOPYOBJ_2D_DECLARATION(i) template <> PyObject* PyPackageParser::toPyObject<Data##i>(void *, ivec2);
#define TOPYOBJ_3D_DECLARATION(i) template <> PyObject* PyPackageParser::toPyObject<Data##i>(void *, ivec3);

//Two Components
#define TOPYOBJ_1D_2COMPONENTS_DECLARATION(i) template <> PyObject* PyPackageParser::toPyObject<DataVec2##i>(void *, size_t);
#define TOPYOBJ_2D_2COMPONENTS_DECLARATION(i) template <> PyObject* PyPackageParser::toPyObject<DataVec2##i>(void *, ivec2);
#define TOPYOBJ_3D_2COMPONENTS_DECLARATION(i) template <> PyObject* PyPackageParser::toPyObject<DataVec2##i>(void *, ivec3);

//Three Components
#define TOPYOBJ_1D_3COMPONENTS_DECLARATION(i) template <> PyObject* PyPackageParser::toPyObject<DataVec3##i>(void *, size_t);
#define TOPYOBJ_2D_3COMPONENTS_DECLARATION(i) template <> PyObject* PyPackageParser::toPyObject<DataVec3##i>(void *, ivec2);
#define TOPYOBJ_3D_3COMPONENTS_DECLARATION(i) template <> PyObject* PyPackageParser::toPyObject<DataVec3##i>(void *, ivec3);

//Four Components
#define TOPYOBJ_1D_4COMPONENTS_DECLARATION(i) template <> PyObject* PyPackageParser::toPyObject<DataVec4##i>(void *, size_t);
#define TOPYOBJ_2D_4COMPONENTS_DECLARATION(i) template <> PyObject* PyPackageParser::toPyObject<DataVec4##i>(void *, ivec2);
#define TOPYOBJ_3D_4COMPONENTS_DECLARATION(i) template <> PyObject* PyPackageParser::toPyObject<DataVec4##i>(void *, ivec3);

class IVW_MODULE_PYPACKAGES_API PyPackageParser {
    PyPackageParser();
public:
    template <typename T> static T parse(PyObject* arg);
    template <typename T> static PyObject* toPyObject(void* ptr, size_t dim); //1d
    template <typename T> static PyObject* toPyObject(void* ptr, ivec2 dim); //2d
    template <typename T> static PyObject* toPyObject(void* ptr, ivec3 dim); //3d
    template<typename T> static bool is(PyObject* arg);
};

template <> void IVW_MODULE_PYPACKAGES_API *PyPackageParser::parse(PyObject* args);

//Single component
//1d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_1D_DECLARATION)
//2d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_2D_DECLARATION)
//3d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_3D_DECLARATION)


//Two components
//1d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_1D_2COMPONENTS_DECLARATION)
//2d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_2D_2COMPONENTS_DECLARATION)
//3d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_3D_2COMPONENTS_DECLARATION)


//Three Components
//1d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_1D_3COMPONENTS_DECLARATION)
//2d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_2D_3COMPONENTS_DECLARATION)
//3d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_3D_3COMPONENTS_DECLARATION)


//Four Components
//1d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_1D_4COMPONENTS_DECLARATION)
//2d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_2D_4COMPONENTS_DECLARATION)
//3d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_3D_4COMPONENTS_DECLARATION)

template <> bool IVW_MODULE_PYPACKAGES_API PyPackageParser::is<void*>(PyObject* arg);


} //namespace

#endif // IVW_PYVALUEPARSERINVIWO_H
