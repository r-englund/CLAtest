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
 *********************************************************************************/

#include <modules/python/pythoninterface/pymodule.h>

#include <modules/python/pyinviwo.h>
#include <inviwo/core/util/logcentral.h>
#include <modules/python/pythoninterface/pyvalueparser.h>

namespace inviwo {

std::map<PyObject*,PyModule*> PyModule::instances_;

PyModule::PyModule(std::string moduleName,std::vector<PyMethod*> methods):moduleName_(moduleName) {
    moduleObject_ = PyInviwo::getPtr()->registerPyModule(this);

    if (!moduleObject_) {
        LogWarn("Module could not be initzialied ");
        return;
    }

    instances_[moduleObject_] = this;
    name_ = PyValueParser::toPyObject(moduleName_);
    dict_ = PyModule_GetDict(moduleObject_);
    std::vector<PyMethod*>::iterator it;

    for (it = methods.begin(); it!=methods.end(); ++it)
        addMethod(*it);

    addMethod(new PyInfoMethod());
}

PyModule::~PyModule() {
    while (!methods_.empty()) {delete methods_.back(); methods_.pop_back();}
}

void PyModule::addMethod(PyMethod* method) {
    methods_.push_back(method);
    PyObject* pyFunc = PyCFunction_NewEx(method->getDef(),moduleObject_,name_);
    PyDict_SetItemString(dict_, method->getName().c_str(), pyFunc);
    Py_DECREF(pyFunc);
}

const char* PyModule::getModuleName() {return moduleName_.c_str();}

void PyModule::printInfo() {
    for (unsigned int i = 0; i<methods_.size(); i++) {
        std::string msg = "print \"";
        msg += methods_[i]->getDesc();
        msg += "\" ";
        PyRun_SimpleString(msg.c_str());
        PyRun_SimpleString("print \" \"");
    }
}

std::vector<PyMethod*> PyModule::getPyMethods() {
    return methods_;
}

PyModule* PyModule::getModuleByPyObject(PyObject* obj) {
    return instances_[obj];
}



PyObject* py_info(PyObject* self, PyObject* args) {
    static PyInfoMethod p;

    if (!p.testParams(args))
        return 0;

    inviwo::PyModule::getModuleByPyObject(self)->printInfo();
    Py_RETURN_NONE;
}


}//namespace