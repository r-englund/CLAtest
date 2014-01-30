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

#ifndef IVW_PYMODULE_H
#define IVW_PYMODULE_H

#include <modules/python/pythonmoduledefine.h>

#include "../pythoninterface/pymethod.h"

#include <vector>
#include <string>
#include <map>

PyObject* py_info(PyObject* /*self*/, PyObject* /*args*/);

namespace inviwo {

    class IVW_MODULE_PYTHON_API PyInfoMethod : public PyMethod{
    public:
        std::string getName(){return "info";}
        std::string getDesc(){return "Prints documentation of the module's functions.";}
        virtual PyCFunction getFunc(){return py_info;}
    };



class IVW_MODULE_PYTHON_API PyModule{

public:
    PyModule(std::string moduleName,std::vector<PyMethod*> methods = std::vector<PyMethod*>());
    ~PyModule();
    void addMethod(PyMethod* method);

    const char* getModuleName();

    void printInfo();

    std::vector<PyMethod*> getPyMethods();

    static PyModule* getModuleByPyObject(PyObject *obj);


private:
    std::string moduleName_;
    std::vector<PyMethod*> methods_;
    PyObject* moduleObject_;
    PyObject* dict_;
    PyObject *name_;

    static std::map<PyObject*,PyModule*> instances_;


};



}//namespace

#endif
