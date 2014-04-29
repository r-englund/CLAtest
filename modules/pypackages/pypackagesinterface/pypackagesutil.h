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

#ifndef IVW_PYPACKAGESUTIL_H
#define IVW_PYPACKAGESUTIL_H


#include <modules/pypackages/pypackagesmoduledefine.h>

#include <modules/python/pythoninterface/pymethod.h>

namespace inviwo {

//Global variable access
PyObject* py_declareGlobalString(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_setGlobalString(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getGlobalString(PyObject* /*self*/, PyObject* /*args*/);

class IVW_MODULE_PYPACKAGES_API PySetGlobalString : public PyMethod {
public:
    PySetGlobalString();
    virtual ~PySetGlobalString() {}
    virtual std::string getName()const {return "setGlobalString";}
    virtual std::string getDesc()const {return "Set global variable that can be accessed by Inviwo.";}
    virtual PyCFunction getFunc() {return py_setGlobalString;}
private:
    PyParamString variableName_;
    PyParamString value_;
};

class IVW_MODULE_PYPACKAGES_API PyGetGlobalString : public PyMethod {
public:
    PyGetGlobalString();
    virtual ~PyGetGlobalString() {}
    virtual std::string getName()const {return "getGlobalString";}
    virtual std::string getDesc()const {return "Get global variable that can be accessed by Inviwo.";}
    virtual PyCFunction getFunc() {return py_getGlobalString;}
    static std::string getGlobalStringValue(std::string variableName);
private:
    PyParamString variableName_;
};


class IVW_MODULE_PYPACKAGES_API PyDeclareGlobalString : public PyMethod {
public:
    PyDeclareGlobalString();
    virtual ~PyDeclareGlobalString() {}
    virtual std::string getName()const {return "declareGlobalString";}
    virtual std::string getDesc()const {return "Allocate global variable that can be accessed with inviwo.";}
    virtual PyCFunction getFunc() {return py_declareGlobalString;}
private:
    PyParamString variableName_;
    PyParamString value_;
};

PyObject* py_declareBufferData(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getBufferData(PyObject* /*self*/, PyObject* /*args*/);

class IVW_MODULE_PYPACKAGES_API PyDeclareBufferData : public PyMethod {
public:
    PyDeclareBufferData();
    virtual ~PyDeclareBufferData() {}
    virtual std::string getName()const {return "declareBufferData";}
    virtual std::string getDesc()const {return "Buffer data being allocated by Inviwo.";}
    virtual PyCFunction getFunc() {return py_declareBufferData;}
private:
    PyParamString pyprocessorId_;
    PyParamString bufferVariableName_;
	PyParamString bufferVariableType_;
	PyParamInt bufferSize_;
};

class IVW_MODULE_PYPACKAGES_API PyGetBufferData : public PyMethod {
public:
    PyGetBufferData();
    virtual ~PyGetBufferData() {}
    virtual std::string getName()const {return "getBufferData";}
    virtual std::string getDesc()const {return "Get buffer allocated by Inviwo.";}
    virtual PyCFunction getFunc() {return py_getBufferData;}
private:
    PyParamString pyprocessorId_;
    PyParamString bufferVariableName_;	
};

} //namespace



#endif // IVW_PYPACKAGESUTIL_H


