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
 *********************************************************************************/

#ifndef IVW_PYPACKAGESUTIL_H
#define IVW_PYPACKAGESUTIL_H


#include <modules/pypackages/pypackagesmoduledefine.h>

#include <modules/python/pythoninterface/pymethod.h>

namespace inviwo {

//////////////////////////////////////////////////////////////////////////
//Buffers
PyObject* py_declareBufferData(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getBufferData(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getLayerData(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_getVolumeData(PyObject* /*self*/, PyObject* /*args*/);

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

class IVW_MODULE_PYPACKAGES_API PyGetLayerData : public PyMethod {
public:
    PyGetLayerData();
    virtual ~PyGetLayerData() {}
    virtual std::string getName()const {return "getLayerData";}
    virtual std::string getDesc()const {return "Get layer allocated by Inviwo.";}
    virtual PyCFunction getFunc() {return py_getLayerData;}
private:
    PyParamString pyprocessorId_;
    PyParamString layerVariableName_;   
};

class IVW_MODULE_PYPACKAGES_API PyGetVolumeData : public PyMethod {
public:
    PyGetVolumeData();
    virtual ~PyGetVolumeData() {}
    virtual std::string getName()const {return "getVolumeData";}
    virtual std::string getDesc()const {return "Get volume allocated by Inviwo.";}
    virtual PyCFunction getFunc() {return py_getVolumeData;}
private:
    PyParamString pyprocessorId_;
    PyParamString volumeVariableName_;   
};

} //namespace



#endif // IVW_PYPACKAGESUTIL_H


