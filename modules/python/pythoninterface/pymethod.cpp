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

#include "../pythoninterface/pymethod.h"

#include "pymodule.h"

namespace inviwo{

    PyParamBase::PyParamBase(std::string paramName,bool optional)
        : name_(paramName)
        , optional_(optional)
    {

    }

std::string PyParamBase::getParamName()const{
    return name_;
}

bool PyParamBase::isOptional()const{
    return optional_;
}

PyMethod::PyMethod():optionalParams_(0){

}


PyMethodDef* PyMethod::getDef(){
    def_.ml_doc   = getDesc().c_str();
    def_.ml_flags = getFlags();
    def_.ml_meth  = getFunc();
    def_.ml_name  = getName().c_str();
    return &def_;
}

std::string PyMethod::getParamDesc(){
    if(params_.empty()){
        return "None";
    }

    std::stringstream ss;
    for(size_t i = 0;i<params_.size();i++){
        if(i!=0)
            ss << " , ";
        if(params_[i]->isOptional())
            ss << "[";
        ss << params_[i]->paramType() << " " << params_[i]->getParamName();

        if(params_[i]->isOptional())
            ss << "]";

    }

    

    return ss.str();
}

bool PyMethod::testParams(PyObject* args)const{
    Py_ssize_t size = PyTuple_Size(args);
    if(static_cast<size_t>(size) > params_.size() || static_cast<size_t>(size) < (params_.size()-optionalParams_)){
        std::stringstream ss;
        ss << getName() << "() expects " << params_.size()-optionalParams_ << " parameters ";
        if(optionalParams_!=0){
            ss << " and may have " << optionalParams_ << " additional parameters";
        }
        PyErr_SetString(PyExc_TypeError,ss.str().c_str());
        return false;
    }

    for(Py_ssize_t i = 0;i<size;i++){
        if(!params_[i]->testParam(PyTuple_GetItem(args,i))){
            std::stringstream ss;
            ss << getName() << "() expects a " << params_[i]->paramType() << " for its " << i << ":th parameter";
            PyErr_SetString(PyExc_TypeError,ss.str().c_str());
            return false;
        }
    }

    return true;
}

void PyMethod::addParam(PyParamBase* param){
    ivwAssert(optionalParams_ == 0 || ( optionalParams_ != 0 && param->isOptional()),"Once one paramter has been marked as optional, the rest of the has to as well");
    params_.push_back(param);
    if(param->isOptional())
       optionalParams_++;     
}



}//namespace