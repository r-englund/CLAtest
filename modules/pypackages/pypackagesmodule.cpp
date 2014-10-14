/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Contact: Sathish Kottravel
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
#include <modules/pypackages/processors/numpy/numpyimagecontour.h>
#include <modules/pypackages/processors/numpy/numpyvolumehistogram.h>
#include <modules/pypackages/processors/numpy/numpybuffertest.h>
#include <modules/pypackages/widgets/pypackagemenu.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#define PY_ARRAY_UNIQUE_SYMBOL PYPACKAGE_ARRAY_API
#include <Python.h>
#include <arrayobject.h>

namespace inviwo {

PyPackagesModule::PyPackagesModule() : InviwoModule() {
    setIdentifier("PyPackages");
    initPyPackagesInterface();    

    //Specify required packages
    //numpy
    PyScriptRunner::getPtr()->addModulePackageRequirement(this, "numpy");

    //matplot & dependencies
    PyScriptRunner::getPtr()->addModulePackageRequirement(this, "matplotlib");
    PyScriptRunner::getPtr()->addModulePackageRequirement(this, "python-dateutil");
    PyScriptRunner::getPtr()->addModulePackageRequirement(this, "pytz");
    PyScriptRunner::getPtr()->addModulePackageRequirement(this, "pyparsing");
    PyScriptRunner::getPtr()->addModulePackageRequirement(this, "six");

    //pycuda & dependencies
    PyScriptRunner::getPtr()->addModulePackageRequirement(this, "pycuda");
    PyScriptRunner::getPtr()->addModulePackageRequirement(this, "pytools");

    //check if package available
    bool numpyAvailable = numpyRequirement();
    bool matplotAvailable = matplotlibRequirement();
    bool pyCUDAAvailable = pycudaRequirement();

    //register if required package is available
    if (numpyAvailable) {
        registerProcessor(NumpyBufferTest);
    }

    if (numpyAvailable && matplotAvailable) {
        registerProcessor(NumpyImageContour);
        registerProcessor(NumpyVolumeHistogram);
    }

    if (pyCUDAAvailable) {
        registerProcessor(PyCUDAImageInverter);
    }

    pyPackageMenu_ = new PyPackageMenu();
}

PyPackagesModule::~PyPackagesModule() {
    delete pyPackageMenu_;
}

void PyPackagesModule::initPyPackagesInterface() {
    PythonModule* baseModule = static_cast<PythonModule*>(InviwoApplication::getPtr()->getModuleByType<PythonModule>());

    PyInviwo* pyInviwo = baseModule->getPyInviwo();

    PyModule* pyPackageMod = new PyModule("pypackagesutil");
    pyInviwo->registerPyModule(pyPackageMod);
    pyPackageMod->addMethod(new PyDeclareBufferData());
    pyPackageMod->addMethod(new PyGetBufferData());
    pyPackageMod->addMethod(new PyGetLayerData());
    pyPackageMod->addMethod(new PyGetVolumeData());

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

        //ss.clear();
        //ss << "import sys" << std::endl;
        //ss << "prev_sys_path = list(sys.path)" << std::endl;
        //ss << "print prev_sys_path" << std::endl;
        //std::string getSysPath(ss.str());
        ////int ret = PyRun_SimpleString(runString.c_str());
        //PyScriptRunner::getPtr()->run(getSysPath);
        //retError = PyScriptRunner::getPtr()->getError();
        ////std::string path;
        //if (retError=="") {
        //    path = PyScriptRunner::getPtr()->getStandardOutput();
        //    LogInfo("System paths are '" + path + "'");
        //}

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
    }
    else {
        LogError("Python is not Initialized. Hence numpy not initialized.");
    }
}

bool PyPackagesModule::numpyRequirement() {
    bool isNumpyAvailable = PyScriptRunner::getPtr()->isPackageAvailable("numpy");
    if (!isNumpyAvailable) LogWarn("Numpy not available");
    return isNumpyAvailable;
}

bool PyPackagesModule::matplotlibRequirement() {
    bool isMatplotAvailable = PyScriptRunner::getPtr()->isPackageAvailable("matplotlib");
    if (!isMatplotAvailable) LogWarn("Matplotlib not available");
    return isMatplotAvailable;
}

bool PyPackagesModule::pycudaRequirement() {
    bool isPycudaAvailable = PyScriptRunner::getPtr()->isPackageAvailable("pycuda");
    if (!isPycudaAvailable) LogWarn("PyCUDA not available")
    return isPycudaAvailable;
}

} // namespace