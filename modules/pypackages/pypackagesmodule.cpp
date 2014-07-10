/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013 Inviwo Foundation
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

#include <modules/pypackages/pypackagesmodule.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/common/inviwomodule.h>
#include <modules/python/pythonmodule.h>
#include <modules/python/pythoninterface/pymodule.h>
#include <modules/python/pyinviwo.h>
#include <modules/pypackages/pypackagesinterface/pypackagesutil.h>
#include <modules/pypackages/processors/pycuda/pycudaimageinvert.h>
#include <modules/pypackages/processors/numpy/numpybuffertest.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#define PY_ARRAY_UNIQUE_SYMBOL PYPACKAGE_ARRAY_API
#include <Python.h>
#include <arrayobject.h>

namespace inviwo {

PyPackagesModule::PyPackagesModule() : InviwoModule() {
    setIdentifier("PyPackages");
    initPyPackagesInterface();
    registerProcessor(NumpyBufferTest);
    registerProcessor(PyCUDAImageInverter);
}

void PyPackagesModule::initPyPackagesInterface() {
    PythonModule* baseModule = static_cast<PythonModule*>(InviwoApplication::getPtr()->getModuleByType<PythonModule>());

    PyInviwo* pyInviwo = baseModule->getPyInviwo();

    PyModule* pyPackageMod = new PyModule("pypackagesutil");
    pyInviwo->registerPyModule(pyPackageMod);
    pyPackageMod->addMethod(new PyDeclareBufferData());
    pyPackageMod->addMethod(new PyGetBufferData());
    pyPackageMod->addMethod(new PyGetLayerData());

    pyScriptRunner_ = new PyScriptRunner();

    if (Py_IsInitialized()) {

        //Find path to site-packages 
        std::string retError;
        std::stringstream ss;
        ss << "import site; sitePackagePath = \'\'" << std::endl;
        ss << "for packagePath in site.getsitepackages():" << std::endl;
        ss << "    if \'site\' in packagePath or \'packages\' in packagePath: sitePackagePath=packagePath" << std::endl;
        ss << "print sitePackagePath" << std::endl;

        std::string getSitePackagePathScript(ss.str());         
        //int ret = PyRun_SimpleString(runString.c_str());
        PyScriptRunner::getPtr()->run(getSitePackagePathScript);
        retError = PyScriptRunner::getPtr()->getError();
        std::string path;
        if (retError=="") {
            path = PyScriptRunner::getPtr()->getStandardOutput();
            LogInfo("Site package path is found at '" + path + "'");
            //path = std::string("C:\\Python27x64\\Lib\\site-packages");
        }

        //Append site-package path to system path
        std::string pathConv = path;
        replaceInString(pathConv, "\\", "/");
        std::string setSystemPathScript = "import sys\n";
        setSystemPathScript.append(std::string("sys.path.append('") + pathConv + std::string("')"));        
        PyScriptRunner::getPtr()->run(setSystemPathScript);
        retError = PyScriptRunner::getPtr()->getError();
        if (retError!="") {
            LogWarn("Failed to add '" + pathConv + "' to Python module search path");
        }
        else {
            LogInfo("Successfully added site package path '" + pathConv + "' to Python module search path");
        }

        //Initialize numpy
        import_array();

        //Retrive available packages
        isPackageAvailable("pycuda");
        isPackageAvailable("numpy");
    }
    else {
        LogError("Python is not Initialized. Hence numpy not initialized.");
    }
}

bool PyPackagesModule::isPackageAvailable(std::string packageName) {
    
    std::stringstream ss;
    std::string packageAvailable = packageName + " is available";
    std::string packageNotAvailable = packageName + " is not available";
    ss << "import imp "<< std::endl; 
    ss << "try: "<< std::endl; 
    ss << "    imp.find_module('" + packageName +"') "<< std::endl;
    ss << "    print '" + packageAvailable << "'"<< std::endl;
    ss << "except ImportError:" << std::endl;
    ss << "    print '" + packageNotAvailable << "'"<< std::endl;

    std::string queryPackage(ss.str());
    PyScriptRunner::getPtr()->run(queryPackage);
    std::string retError = PyScriptRunner::getPtr()->getError();
    
    bool found = false;
    if (retError!="") {
        LogWarn("Module query failed");
        found = false;
    }
    else {
        std::string status = PyScriptRunner::getPtr()->getStandardOutput();
        if (status==packageAvailable) {
            LogInfo("Pacakage " + packageName + " is available");
            found = true;
        }
        else {
            LogInfo("Pacakage " + packageName + " is not available");
            found = false;
        }
    }
     PyScriptRunner::getPtr()->clear();
    return found;
}

} // namespace
