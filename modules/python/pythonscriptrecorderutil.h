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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

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
