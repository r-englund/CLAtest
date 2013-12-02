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

#ifndef IVW_PYTHON_SCRIPT_H
#define IVW_PYTHON_SCRIPT_H

#ifdef IVW_PYTHON
    #ifdef _DEBUG
        #undef _DEBUG //Prevent linking debug build of python
            #include <Python.h>
        #define _DEBUG 1
    #else
        #include <Python.h>
    #endif
#else
    class PyObject;
#endif


#include <string>

#include <modules/python/pythonmoduledefine.h>

namespace inviwo {

/**
 * Class for handling storage, compile and running of Python Scripts.
 * Used by PythonScriptEditor and PythonModule
 * 
 */
class IVW_MODULE_PYTHON_API PythonScript {

public:
    PythonScript();

    /**
     * Frees the stored byte code. Make sure that the
     * Python interpreter is still initialized
     * when deleting the script.
     */
    ~PythonScript();

    /**
     * Sets the source for the Python (replacing the current source).
     */
    void setSource(const std::string& source);

    /**
     * Returns the script's source.
     */
    std::string getSource() const;

    /**
     * Runs the script once,
     * if the script has changed since last compile a new compile call will be issued.
     *
     * If an error occurs, the error message is logged to the inviwo logger and python standard output.
     *
     * @return true, if script execution has been successful
     */
    bool run();

private:
    bool checkCompileError();
    bool checkRuntimeError();

    /**
     * Compiles the script source to byte code, which speeds up script execution
     * and is generally recommended, since it also produces more clear error messages.
     *
     * If an error occurs, the error message is stored and can be retrieved through getLog().
     *
     * @param logErrors if true, error messages are also passed to the logger.
     *  The internal log buffer is not affected by this parameter.
     *
     * @return true, if script compilation has been successful
     */
    bool compile();

    std::string     source_;
    PyObject*       byteCode_;
    bool            isCompileNeeded_;
};

}

#endif // VRN_PYTHONSCRIPT_H
