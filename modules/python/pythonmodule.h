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
    PyInviwo *pyInviwo_;
};

} // namespace

#endif // IVW_PYTHON_MODULE_H
