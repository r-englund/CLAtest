/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.9
 *
 * Copyright (c) 2013-2015 Inviwo Foundation
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

#ifndef IVW_PYMODULE_H
#define IVW_PYMODULE_H

#include <modules/python/pythonmoduledefine.h>

#include <modules/python/pythoninterface/pymethod.h>

#include <vector>
#include <string>
#include <map>


namespace inviwo {

PyObject* py_info(PyObject* /*self*/, PyObject* /*args*/);

class IVW_MODULE_PYTHON_API PyInfoMethod : public PyMethod {
public:
    virtual std::string getName()const {return "info";}
    virtual std::string getDesc()const {return "Prints documentation of the module's functions.";}
    virtual PyCFunction getFunc() {return py_info;}
};



class IVW_MODULE_PYTHON_API PyModule {
public:
    PyModule(std::string moduleName,std::vector<PyMethod*> methods = std::vector<PyMethod*>());
    ~PyModule();
    void addMethod(PyMethod* method);

    const char* getModuleName();

    void printInfo();

    std::vector<PyMethod*> getPyMethods();

    static PyModule* getModuleByPyObject(PyObject* obj);

private:
    std::string moduleName_;
    std::vector<PyMethod*> methods_;
    PyObject* moduleObject_;
    PyObject* dict_;
    PyObject* name_;

    static std::map<PyObject*,PyModule*> instances_;


};


}//namespace

#endif
