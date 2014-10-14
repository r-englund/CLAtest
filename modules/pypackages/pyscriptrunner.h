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

#ifndef IVW_PYTHONSCRIPTIRUNNER_H
#define IVW_PYTHONSCRIPTIRUNNER_H

#include <modules/pypackages/pypackagesmoduledefine.h>
#include <modules/python/pythonscript.h>
#include <modules/python/pythonexecutionoutputobeserver.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_MODULE_PYPACKAGES_API PyScriptRunner : public Singleton<PyScriptRunner>, public PythonExecutionOutputObeserver { 
public:
    PyScriptRunner();
    virtual ~PyScriptRunner();

    //Compile/Run Script
    virtual void onPyhonExecutionOutput(const std::string &msg,const PythonExecutionOutputStream &outputType);
    void setScript(std::string simplePythonScript);    
    void run(bool noLogging=true);
    void run(std::string simplePythonScript, bool noLogging=true);
    std::string getStandardOutput();
    std::string getError();
    void clear();

    //Package management    
    bool isPipInstalled();
    std::string getPackageVersionInfo(std::string packageName);
    bool isPackageAvailable(std::string packageName, bool forceAvailabilityCheck=false);
    bool installPackage(std::string packageName);
    bool uninstallPackage(std::string packageName);
    bool upgradePackage(std::string packageName);
    void addModulePackageRequirement(InviwoModule* module, std::string packageName);
    std::map<InviwoModule*, std::vector<std::string> > getModulePackageRequirement() const;
private:
    std::string parsePipUtil();
    bool isModuleAvailable(std::string moduleName);
    PythonScript script_;
    std::string error_;
    std::string standard_;
    std::vector<std::string> availablePacakges_;
    std::map<InviwoModule*, std::vector<std::string> > modulePacakgeRequirements_;
};

}//namespace

#endif //IVW_PYTHONSCRIPTIRUNNER_H