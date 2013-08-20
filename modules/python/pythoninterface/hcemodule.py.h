#ifndef IVW_HCEMODULEINVIWO_H
#define IVW_HCEMODULEINVIWO_H

#ifndef IVW_PYINVIWO_CPP
    #error This file should only be included from pyinviwo.cpp
#endif

#include <modules/python/pythonmoduledefine.h>
#include <modules/humancomputationengine/humancomputationenginemodule.h>
#include <modules/humancomputationengine/crowdflowersettings.h>
#include <modules/humancomputationengine/crowdflowerjob.h>

#include "pythonMethod.h"

static PyObject* py_setCrowdFlowerSettingFile(PyObject* /*self*/, PyObject* /*args*/);
static PyObject* py_submitJob(PyObject* /*self*/, PyObject* /*args*/);
static PyObject* py_addUnitFile(PyObject* /*self*/, PyObject* /*args*/);

std::vector<std::string> fileNames_;

namespace inviwo {
    class PySetCrowdFlowerSettingFileMethod : public PythonMethod{
    public:
        char *getName(){return "setCrowdFlowerSettingFile";}
        char *getDesc(){return "setCrowdFlowerSettingFile()\tTake setting files with key as input";}
        virtual PyCFunction getFunc(){return py_setCrowdFlowerSettingFile;}
    };

    class PySubmitJobMethod : public PythonMethod{
    public:
        char *getName(){return "submitJob";}
        char *getDesc(){return "submitJob(unit_file, question_file, job_title, description, instruction)\tSubmits a job to crowdflower platform";}
        virtual PyCFunction getFunc(){return py_submitJob;}
    };

    class PyAddUnitFileMethod : public PythonMethod{
    public:
        char *getName(){return "addUnitFile";}
        char *getDesc(){return "addUnitFile(unit_file)\tAdds Unit file to a job";}
        virtual PyCFunction getFunc(){return py_addUnitFile;}
    };

} //namespace


static PyObject* py_setCrowdFlowerSettingFile(PyObject* /*self*/, PyObject* args){
    
    if (PyTuple_Size(args) != 1) {
         std::ostringstream errStr;
         errStr << "setCrowdFlowerSettingFile(fileName) takes exactly 1 argument: settings file name";
         errStr << " (" << PyTuple_Size(args) << " given)";
         PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
         return 0;
    }
    

    if(InviwoApplication::getPtr()){
        HumanComputationEngineModule* hceModule = 0;
        std::vector<InviwoModule*> modules = InviwoApplication::getPtr()->getModules();        
        for (size_t i=0; i<modules.size(); i++) {
            hceModule = dynamic_cast<HumanComputationEngineModule*>(modules[i]);
            if (hceModule) break;
        }       
        
        if (hceModule) {
            //std::cout<<"Found HCE module..." << std::endl;
            char* settingsFileName;
            int len;
            if(!PyArg_ParseTuple(args,"s#", &settingsFileName, &len)) {
                std::ostringstream errStr;
                errStr << "setCrowdFlowerSettingFile(fileName) takes exactly 1 argument: settings file name";
                PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
                return 0;
            }

            CrowdEngineSettings* settings = hceModule->getCrowdFlowerSettings();
            std::string fileName(settingsFileName);
            settings->loadSettings(fileName);
            settings->testAunthenticationButton_->invokeMemberFunctions();           
        }
        else
            return 0;
    }

    Py_RETURN_NONE;
}



static PyObject* py_submitJob(PyObject* /*self*/, PyObject* args){

    if (PyTuple_Size(args) != 5) {
        std::ostringstream errStr;
        errStr << "submitJob(fileName) takes exactly 5 argument: unit_file, question_file, job_title, description, instruction";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    HumanComputationEngineModule* hceModule = 0;
    std::vector<InviwoModule*> modules = InviwoApplication::getPtr()->getModules();        
    for (size_t i=0; i<modules.size(); i++) {
        hceModule = dynamic_cast<HumanComputationEngineModule*>(modules[i]);
        if (hceModule) break;
    }       

    if (hceModule) {
        //std::cout<<"Found HCE module..." << std::endl;
        char* unitFile; int uintFileLen;
        char* questionFile; int questionFileLen;
        char* jobTitle; int jobTitleLen;
        char* jobDescription; int jobDescriptionLen;
        char* jobInstruction; int jobInstructionLen;
        if(!PyArg_ParseTuple(args,"s#s#s#s#s#", &unitFile, &uintFileLen
                                              , &questionFile, &questionFileLen
                                              , &jobTitle, &jobTitleLen
                                              , &jobDescription, &jobDescriptionLen
                                              , &jobInstruction, &jobInstructionLen)) {
            std::ostringstream errStr;
            errStr << "submitJob(fileName) takes exactly 5 argument: unit_file, question_file, job_title, description, instruction";
            PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
            return 0;
        }       

        IvwCrowdEngineJob* crowdJob = new IvwCrowdEngineJob();
        
        crowdJob->data_.set(unitFile);
        crowdJob->question_.set(questionFile);
        crowdJob->title_.set(jobTitle);
        crowdJob->description_.set(jobDescription);
        crowdJob->instructions_.set(jobInstruction);
        if (uintFileLen>1)
            crowdJob->submitJob();
        else if (fileNames_.size())
            crowdJob->submitJob(fileNames_);
    }
    else
        return 0;

    Py_RETURN_NONE;
}


static PyObject* py_addUnitFile(PyObject* /*self*/, PyObject* args){

    if (PyTuple_Size(args) != 1) {
        std::ostringstream errStr;
        errStr << "py_addUnitFile(fileName) takes exactly 1 argument: unit_file";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    
    char* unitFile; int uintFileLen;
    
    if(!PyArg_ParseTuple(args,"s#", &unitFile, &uintFileLen)) {
            std::ostringstream errStr;
            errStr << "submitJob(fileName) takes exactly 1 argument: unit_file";
            PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
            return 0;
    }       

    if (uintFileLen<=1)
        fileNames_.clear();
    else
        fileNames_.push_back(std::string(unitFile));    
    

    Py_RETURN_NONE;
}

#endif // #ifndef IVW_HCEMODULEINVIWO_H


