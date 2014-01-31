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

#define IVW_PYINVIWO_CPP

#include "pythonmodule.h"
#include "pyinviwo.h"

#include "pythonscript.h"

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/boolproperty.h>

#include <modules/opengl/canvasprocessorgl.h>

using namespace inviwo;

#include "defaultinterface/pyproperties.h"
#include "defaultinterface/pycamera.h"
#include "defaultinterface/pycanvas.h"
#include "defaultinterface/pylist.h"
#include "defaultinterface/pyutil.h"
#include "defaultinterface/pyvolume.h"
#include "pythonexecutionoutputobeserver.h"

static PyObject* py_stdout(PyObject* /*self*/, PyObject* args) {
    char* msg;
    int len;
    int isStderr;
    if (!PyArg_ParseTuple(args, "s#i", &msg, &len, &isStderr)) {
        LogWarnCustom("inviwo.Python.py_print", "failed to parse log message");
    }
    else {
        if(len!=0){
            if(!(len==1 && (msg[0] == '\n' || msg[0] == '\r' || msg[0] == '\0')))
                PythonExecutionOutputObeserver::pyhonExecutionOutputEvent(msg,PythonExecutionOutputObeserver::standard);
        }
    }
    Py_RETURN_NONE;
}




namespace inviwo{
    class PyStdOutCatcher : public PyMethod{
    public:
        virtual std::string getName()const{return "ivwPrint";}
        virtual std::string getDesc()const{return " Only for internal use. Redirect std output to python editor widget.";}
        virtual PyCFunction getFunc(){return py_stdout;}
    };


    PyInviwo::PyInviwo():
        isInit_(false){
        init(this);
        init_();
    }

    PyInviwo::~PyInviwo(){
        delete inviwoPyModule;
        delete inviwoInternalPyModule;
        Py_Finalize();
    }

    void PyInviwo::init_() {
        if(isInit_)
            return;
        isInit_ = true;

        LogInfo("Python version: " + toString(Py_GetVersion()));
#if PY_MAJOR_VERSION <= 2
        char programName[] = "PyInviwo";
#else   
        wchar_t programName[] = L"PyInviwo";
#endif
        Py_SetProgramName(programName);

#ifdef WIN32
        Py_NoSiteFlag = 1;
#endif

        Py_InitializeEx(false);
        if(!Py_IsInitialized()){
            LogError("Python is not Initialized");
            return;
        }
        PyEval_InitThreads();

        addModulePath(InviwoApplication::getPtr()->getBasePath() + "modules/python/scripts");

        initDefaultInterfaces();

        PythonScript outputCatcher = PythonScript();

        std::string directorFileName = InviwoApplication::getPtr()->getBasePath() +"modules/python/scripts/outputredirector.py";
        std::ifstream file(directorFileName.c_str());
        std::string text((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
        file.close();
        outputCatcher.setSource(text);
        if(!outputCatcher.run(false)){
            LogWarn("Python init script failed to run");
        }
    }


    void PyInviwo::initDefaultInterfaces(){
        inviwoPyModule = new PyModule("inviwo");
        inviwoInternalPyModule = new PyModule("inviwo_internal");

        inviwoPyModule->addMethod(new PySetPropertyValueMethod());
        inviwoPyModule->addMethod(new PySetPropertyMaxValueMethod());
        inviwoPyModule->addMethod(new PySetPropertyMinValueMethod());
        inviwoPyModule->addMethod(new PyGetPropertyValueMethod());
        inviwoPyModule->addMethod(new PyGetPropertyMaxValueMethod());
        inviwoPyModule->addMethod(new PyGetPropertyMinValueMethod());
        inviwoPyModule->addMethod(new PyClickButtonMethod());
        inviwoPyModule->addMethod(new PySetCameraFocusMethod());
        inviwoPyModule->addMethod(new PySetCameraUpMethod());
        inviwoPyModule->addMethod(new PySetCameraPosMethod());
        inviwoPyModule->addMethod(new PyListPropertiesMethod());
        inviwoPyModule->addMethod(new PyListProcessorsMethod());
        inviwoPyModule->addMethod(new PyCanvasCountMethod());
        inviwoPyModule->addMethod(new PyResizeCanvasMethod());
        inviwoPyModule->addMethod(new PySnapshotMethod());
        inviwoPyModule->addMethod(new PySnapshotCanvasMethod());
        inviwoPyModule->addMethod(new PyGetBasePathMethod());
        inviwoPyModule->addMethod(new PyQuitInviwoMethod());

        inviwoPyModule->addMethod(new PyGetWorkspaceSavePathMethod());
        inviwoPyModule->addMethod(new PyGetVolumePathMethod());
        inviwoPyModule->addMethod(new PyGetImagePathMethod());
        inviwoPyModule->addMethod(new PyGetModulePathMethod());
        inviwoPyModule->addMethod(new PyGetTransferFunctionPath());

        inviwoPyModule->addMethod(new PySetVoxelMethod());
        inviwoPyModule->addMethod(new PyGetVolumeDimension());

        inviwoPyModule->addMethod(new PyEnableEvaluation());
        inviwoPyModule->addMethod(new PyDisableEvaluation());

        inviwoPyModule->addMethod(new PySaveTransferFunction());
        inviwoPyModule->addMethod(new PyLoadTransferFunction());
        inviwoPyModule->addMethod(new PyClearTransferfunction());
        inviwoPyModule->addMethod(new PyAddTransferFunction());

        inviwoInternalPyModule->addMethod(new PyStdOutCatcher());

    }

    void PyInviwo::addModulePath(const std::string& path) {
        init_();
        if (!Py_IsInitialized()) {
            LogWarn("addModulePath(): not initialized");
            return;
        }

        std::string pathConv = path;
        replaceInString(pathConv, "\\", "/");

        std::string runString = "import sys\n";
        runString.append(std::string("sys.path.append('") + pathConv + std::string("')"));
        int ret = PyRun_SimpleString(runString.c_str());
        if (ret != 0)
            LogWarn("Failed to add '" + pathConv + "' to Python module search path");
    }

    std::vector<PyModule*> PyInviwo::getAllPythonModules(){
        return registeredModules_;
    }

    PyObject* PyInviwo::registerPyModule(PyModule *pyModule) {
        init_();
        if (Py_IsInitialized()) {
#if PY_MAJOR_VERSION <= 2            
            PyObject* obj = Py_InitModule(pyModule->getModuleName(),NULL);
#else
            PyObject* obj = PyModule_Create(pyModule->getPyModuleDef());
            PyImport_AddModuleObject(obj);
            //PyImport_AppendInittab(pyModule->getModuleName(), pyModule);
#endif
            
            if(!obj){
                LogWarn("Failed to init python module '" << pyModule->getModuleName() <<"' ");
            }

            registeredModules_.push_back(pyModule);
            
            for(ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it) {
                static_cast<PyInviwoObserver*>(*it)->onModuleRegistered(pyModule);
            }


            return obj;
        }else{
            LogError("Python environment not initialized");
            return 0;
        }
    }


}//namespace
