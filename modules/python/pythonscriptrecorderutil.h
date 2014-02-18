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

#ifndef IVW_PYTHON_SCRIPT_RECORDER_UTIL_H
#define IVW_PYTHON_SCRIPT_RECORDER_UTIL_H

#include <modules/python/pythonmoduledefine.h>
#include <inviwo/core/properties/property.h>
#include <string>

namespace inviwo {

class PythonScript;

/**
 * Class for recording changes in network.
 * Used by PythonScript
 * On network change call back functions are triggered by network observers in python module
 *
 */
class IVW_MODULE_PYTHON_API PythonScriptRecorderUtil {

public:
    PythonScriptRecorderUtil(PythonScript*);

    ~PythonScriptRecorderUtil();

    /**
     * Starts recording of script. Any network changes are notified and recorded as script.
     */
    void startRecording();
    /**
     * Ends recording of script.
     */
    void endRecording();
    /**
     * Checks if recording is under process.
     */
    bool isRecording();
    /**
     * Record network changes
     */
    void recordNetworkChanges();
    /**
     * Get py command for setting property
     */
    std::string getPyProperty(Property*);
private:
    bool            recordScript_;
    PythonScript*   script_;
};

}

#endif // IVW_PYTHON_SCRIPT_RECORDER_UTIL_H
