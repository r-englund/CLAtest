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

#ifndef IVW_PYINVIWO_H
#define IVW_PYINVIWO_H

#include <modules/python/pythonmoduledefine.h>

#ifdef _DEBUG
#undef _DEBUG //Prevent linking debug build of python
#include <Python.h>
#define _DEBUG 1
#else
#include <Python.h>
#endif

#include <inviwo/core/util/singleton.h>
#include <inviwo/core/util/observer.h>  

#include "pythoninterface/pymodule.h"

#include "pyinviwoobserver.h"

namespace inviwo {    
    
    



    class IVW_MODULE_PYTHON_API PyInviwo : public Singleton<PyInviwo> , public Observable<PyInviwoObserver>{
    public:
        PyInviwo();
        ~PyInviwo();

        

        /** 
         * \brief initialized python modules
         *
         * Initializes given python module. Can be used called from outside this python module.
         * 
         * @param PyModule *pyModule class containing information and methods for python module
         */
        PyObject* registerPyModule(PyModule *pyModule);

        /** 
         * \brief add a path for where ptyhon scripts will look for modules
         *
         * add a path for where ptyhon scripts will look for modules
         * 
         * @param const std::string& path path to folder
         * @param PyMethodDef * module static array of registered classes in a module
         */
        void addModulePath(const std::string& path);

        
        /** 
        * \brief returns all modules that has been registered 
        *
        * add a path for where ptyhon scripts will look for modules
         * 
         * @param const std::string& path path to folder
         * @param PyMethodDef * module static array of registered classes in a module
         */
        std::vector<PyModule*> getAllPythonModules();
   private:
        PyModule* inviwoPyModule;
        PyModule* inviwoInternalPyModule;
        std::vector<PyModule*> registeredModules_; //No owner
        bool isInit_;
        void init_();/** 
        
        
        
        * \brief Initializes the Inviwos default python modules
         *
         * Initializes the python modules (inviwo and inviwo_internal) used for communication between python and Inviwo. 
         */
         void initDefaultInterfaces();
        
    };

} //namespace

#endif // IVW_PYINVIWO_H
