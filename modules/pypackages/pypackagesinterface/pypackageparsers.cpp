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

namespace inviwo {

void* parsePtr(PyObject* args) {
	return PyCObject_AsVoidPtr(args); 
}

PyPackageParser::PyPackageParser() { }

template <> void* PyPackageParser::parse(PyObject* args) {  return parsePtr(args);    }

//Float
template <> PyObject* PyPackageParser::toPyObject<DataFLOAT16>(void *ptr, size_t dim) {
    npy_intp ndims[1] = {dim};
    PyObject* data = PyArray_SimpleNewFromData(1, ndims, NPY_FLOAT16, ptr);
    return data;
}

template <> PyObject* PyPackageParser::toPyObject<DataFLOAT32>(void *ptr, size_t dim) {
    npy_intp ndims[1] = {dim};
    PyObject* data = PyArray_SimpleNewFromData(1, ndims, NPY_FLOAT32, ptr);
    return data;
}

template <> PyObject* PyPackageParser::toPyObject<DataFLOAT64>(void *ptr, size_t dim) {
    npy_intp ndims[1] = {dim};
    PyObject* data = PyArray_SimpleNewFromData(1, ndims, NPY_FLOAT64, ptr);
    return data;
}

//Int
template <> PyObject* PyPackageParser::toPyObject<DataINT8>(void *ptr, size_t dim) {
    npy_intp ndims[1] = {dim}; 
    PyObject* data = PyArray_SimpleNewFromData(1, ndims, NPY_INT8, ptr);
    return data;
}

template <> PyObject* PyPackageParser::toPyObject<DataINT16>(void *ptr, size_t dim) {
    npy_intp ndims[1] = {dim}; 
    PyObject* data = PyArray_SimpleNewFromData(1, ndims, NPY_INT16, ptr);
    return data;
}

template <> PyObject* PyPackageParser::toPyObject<DataINT32>(void *ptr, size_t dim) {
    npy_intp ndims[1] = {dim}; 
    PyObject* data = PyArray_SimpleNewFromData(1, ndims, NPY_INT32, ptr);
    return data;
}

template <> PyObject* PyPackageParser::toPyObject<DataINT64>(void *ptr, size_t dim) {
    npy_intp ndims[1] = {dim}; 
    PyObject* data = PyArray_SimpleNewFromData(1, ndims, NPY_INT64, ptr);
    return data;
}

//Unsigned Int
template <> PyObject* PyPackageParser::toPyObject<DataUINT8>(void *ptr, size_t dim) {
    npy_intp ndims[1] = {dim}; 
    PyObject* data = PyArray_SimpleNewFromData(1, ndims, NPY_UINT8, ptr);
    return data;
}

template <> PyObject* PyPackageParser::toPyObject<DataUINT16>(void *ptr, size_t dim) {
    npy_intp ndims[1] = {dim}; 
    PyObject* data = PyArray_SimpleNewFromData(1, ndims, NPY_UINT16, ptr);
    return data;
}

template <> PyObject* PyPackageParser::toPyObject<DataUINT32>(void *ptr, size_t dim) {
    npy_intp ndims[1] = {dim}; 
    PyObject* data = PyArray_SimpleNewFromData(1, ndims, NPY_UINT32, ptr);
    return data;
}

template <> PyObject* PyPackageParser::toPyObject<DataUINT64>(void *ptr, size_t dim) {
    npy_intp ndims[1] = {dim}; 
    PyObject* data = PyArray_SimpleNewFromData(1, ndims, NPY_UINT64, ptr);
    return data;
}

template <> bool IVW_MODULE_PYPACKAGES_API PyPackageParser::is<void*>(PyObject* arg) {
    return PyArray_Check(arg);
}

}//namespace
