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
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

#include <modules/pypackages/pyscriptrunner.h>
#include <inviwo/core/util/clock.h>

namespace inviwo {

PyScriptRunner::PyScriptRunner() : script_() { init(this); }

PyScriptRunner::~PyScriptRunner() {}

void PyScriptRunner::onPyhonExecutionOutput(std::string msg,OutputType outputType) {
    if (outputType==PythonExecutionOutputObeserver::standard) {
        if (standard_.empty())
            standard_ += msg;
        else
            standard_ += ("\n" + msg);
    }
    else if (outputType==PythonExecutionOutputObeserver::error) {
        if (error_.empty())
            error_ += msg;
        else
            error_ += ("\n" + msg);
    }
}

void PyScriptRunner::setScript(std::string simpleScript) {    
    script_.setSource(simpleScript);
}

void PyScriptRunner::run(std::string simpleScript, bool noLogging) {
    setScript(simpleScript);
    run(noLogging);
}

void PyScriptRunner::run(bool noLogging) {
    clear();
    Clock c;
    c.start();
    bool ok = script_.run();
    c.stop();

    if (ok) {
        LogInfo("Python Script Executed succesfully");
    }

    LogInfo("Execution time: " << c.getElapsedMiliseconds() << " ms");
}

std::string PyScriptRunner::getStandardOutput() { return standard_;}

std::string PyScriptRunner::getError() { return error_;}

void PyScriptRunner::clear() { error_=""; standard_="";}

bool PyScriptRunner::isPackageAvailable(std::string packageName) {

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
    this->run(queryPackage);
    std::string retError = this->getError();

    bool found = false;
    if (retError!="") {
        LogWarn("Module query failed");
        found = false;
    }
    else {
        std::string status =  this->getStandardOutput();
        if (status==packageAvailable) {
            LogInfo("Pacakage " + packageName + " is available");
            found = true;
        }
        else {
            LogInfo("Pacakage " + packageName + " is not available");
            found = false;
        }
    }
    this->clear();
    return found;
}


} // namespace
