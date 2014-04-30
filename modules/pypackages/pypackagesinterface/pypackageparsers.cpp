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
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

#include <modules/pypackages/pypackagesinterface/pypackageparsers.h>
#include <inviwo/core/util/formats.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#define PY_ARRAY_UNIQUE_SYMBOL PYPACKAGE_ARRAY_API
#define NO_IMPORT_ARRAY
#include <arrayobject.h>

#define TOPYOBJ_1D_DEFINITION(i) \
    template <> PyObject* PyPackageParser::toPyObject<Data##i>(void *ptr, size_t dim) { \
    npy_intp ndims[1] = {dim}; \
    PyObject* data = PyArray_SimpleNewFromData(1, ndims, NPY_##i, ptr); \
    return data; }

#define TOPYOBJ_2D_DEFINITION(i) \
    template <> PyObject* PyPackageParser::toPyObject<Data##i>(void *ptr, ivec2 dim) { \
    npy_intp ndims[2] = {dim.x, dim.y}; \
    PyObject* data = PyArray_SimpleNewFromData(1, ndims, NPY_##i, ptr); \
    return data; }

#define TOPYOBJ_3D_DEFINITION(i) \
    template <> PyObject* PyPackageParser::toPyObject<Data##i>(void *ptr, ivec3 dim) { \
    npy_intp ndims[3] = {dim.x, dim.y, dim.z}; \
    PyObject* data = PyArray_SimpleNewFromData(1, ndims, NPY_##i, ptr); \
    return data; }

namespace inviwo {

void* parsePtr(PyObject* args) {
	return PyCObject_AsVoidPtr(args); 
}

PyPackageParser::PyPackageParser() { }

template <> void* PyPackageParser::parse(PyObject* args) {  return parsePtr(args);    }

//1d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_1D_DEFINITION)

//2d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_2D_DEFINITION)

//3d
#include <modules/pypackages/pypackagesformatsmacro.h>
PYPACKAGES_FORMAT_MACRO_EXPANDER(TOPYOBJ_3D_DEFINITION)


template <> bool IVW_MODULE_PYPACKAGES_API PyPackageParser::is<void*>(PyObject* arg) {
    return PyArray_Check(arg);
}

}//namespace
