/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.9
 *
 * Copyright (c) 2013-2015 Inviwo Foundation
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
 *********************************************************************************/

#ifndef IVW_PYTHON_MODULE_H
#define IVW_PYTHON_MODULE_H

#include <modules/python/pythonmoduledefine.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo {

class PyInviwo;

class IVW_MODULE_PYTHON_API PythonModule : public InviwoModule {

public:
    /**
     * \brief Creates the Python module
     *
     * Creates the Python inviwo-module. Creates the PythonEditorWidget and initializes the python interpreter.
     *
     */
    PythonModule();

    /**
     * \brief Destructor for PythonModule
     *
     * Calls the destructor for the PythonEditorWidget and PyInviwo and then shut down the python interpreter.
     */
    virtual ~PythonModule();

    /**
     * \brief get PyInviwo object manages default registration of default modules
     *
     * gets PyInviwo used by python module
     *
     * @return PyInviwo* address of PyInviwo instance
     */
    PyInviwo* getPyInviwo() {return pyInviwo_;}

protected:
    /**
     * \brief Add a path where python scripts looks for modules
     *
     * Add a path where python scripts looks for modules
     *
     * @param const std::string & path Path to directory containing python modules
     */
    void addModulePath(const std::string& path);

private:
    PyInviwo* pyInviwo_;
};

} // namespace

#endif // IVW_PYTHON_MODULE_H
