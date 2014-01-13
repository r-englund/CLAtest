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
#include <inviwo/core/util/logdistributor.h>

namespace inviwo{

    std::map<PyObject*,PyModule*> PyModule::instances_;

PyModule::PyModule(std::string moduleName,std::vector<PyMethod*> methods):moduleName_(moduleName){
    moduleObject_ = PyInviwo::getPtr()->registerPyModule(this);
    if(!moduleObject_){
        LogWarn("Module could not be initzialied ");
        return;
    }

    instances_[moduleObject_] = this;

    name_ = PyString_FromString(moduleName_.c_str());
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



PyModule* PyModule::getModuleByPyObject(PyObject *obj){
    return instances_[obj];
}



}//namespace




PyObject* py_info(PyObject* self, PyObject* args){
    inviwo::PyModule::getModuleByPyObject(self)->printInfo();
    Py_RETURN_NONE;
}
