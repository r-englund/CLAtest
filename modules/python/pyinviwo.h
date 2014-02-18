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
 * Main file authors: Rickard Englund, Sathish Kottravel
 *
 *********************************************************************************/

#ifndef IVW_PYINVIWO_H
#define IVW_PYINVIWO_H

#include <modules/python/pythonmoduledefine.h>

#ifdef IVW_MODULE_PYTHON_EXPORTS
#pragma warning(push)
#pragma warning(disable: 4273)
#ifdef _DEBUG
#undef _DEBUG //Prevent linking debug build of python
#include <Python.h>
#define _DEBUG 1
#else
#include <Python.h>
#endif
#pragma warning(pop)
#else

#endif
#include <inviwo/core/util/singleton.h>
#include <inviwo/core/util/observer.h>

#include "pythoninterface/pymodule.h"

#include "pyinviwoobserver.h"

namespace inviwo {





class IVW_MODULE_PYTHON_API PyInviwo : public Singleton<PyInviwo> , public Observable<PyInviwoObserver> {
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
    PyObject* registerPyModule(PyModule* pyModule);

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
