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

#include "pymodule.h"

#include "../pyinviwo.h"
#include <inviwo/core/util/logcentral.h>
#include "pyvalueparser.h"

namespace inviwo{

    std::map<PyObject*,PyModule*> PyModule::instances_;

PyModule::PyModule(std::string moduleName,std::vector<PyMethod*> methods):moduleName_(moduleName){
#if PY_MAJOR_VERSION >= 3
    struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
    };

    pyModuleDef_.m_base = moduledef.m_base;
    pyModuleDef_.m_clear = NULL;
    pyModuleDef_.m_doc = NULL;
    pyModuleDef_.m_free =NULL;
    pyModuleDef_.m_methods = NULL;
    pyModuleDef_.m_name = getModuleName();
    pyModuleDef_.m_reload = NULL;
    pyModuleDef_.m_size = -1;
    pyModuleDef_.m_traverse = NULL;
#endif    
    moduleObject_ = PyInviwo::getPtr()->registerPyModule(this);
    if(!moduleObject_){
        LogWarn("Module could not be initzialied ");
        return;
    }

    instances_[moduleObject_] = this;
    
    name_ = PyValueParser::toPyObject(moduleName_);
    dict_ = PyModule_GetDict(moduleObject_);

    std::vector<PyMethod*>::iterator it;
    for(it = methods.begin();it!=methods.end();++it)
        addMethod(*it);
    addMethod(new PyInfoMethod());
}

PyModule::~PyModule(){
    while(!methods_.empty()){delete methods_.back();methods_.pop_back();}
}

void PyModule::addMethod(PyMethod* method){
    methods_.push_back(method);

    PyObject* pyFunc = PyCFunction_NewEx(method->getDef(),moduleObject_,name_);
    int a = PyDict_SetItemString(dict_, method->getName().c_str(), pyFunc);
    Py_DECREF(pyFunc);
}

const char* PyModule::getModuleName(){return moduleName_.c_str();}

void PyModule::printInfo(){
    for(unsigned int i = 0;i<methods_.size();i++){
        std::string msg = "print \"";
        msg += methods_[i]->getDesc();
        msg += "\" ";
        PyRun_SimpleString(msg.c_str());
        PyRun_SimpleString("print \" \"");
    }
}

std::vector<PyMethod*> PyModule::getPyMethods(){
    return methods_;
}

PyModule* PyModule::getModuleByPyObject(PyObject *obj){
    return instances_[obj];
}

#if PY_MAJOR_VERSION >= 3
PyModuleDef* PyModule::getPyModuleDef(){
    return &pyModuleDef_;
}
#endif





PyObject* py_info(PyObject* self, PyObject* args){
    static PyInfoMethod p;
    if(p.testParams(args))
        return 0;
    inviwo::PyModule::getModuleByPyObject(self)->printInfo();
    Py_RETURN_NONE;
}


 }//namespace