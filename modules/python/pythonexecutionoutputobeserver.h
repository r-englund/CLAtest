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

#ifndef IVW_PYTHONEXECUTIONOUTPUTOBESERVER_MODULE_H
#define IVW_PYTHONEXECUTIONOUTPUTOBESERVER_MODULE_H

#include <modules/python/pythonmoduledefine.h>

namespace inviwo {
class IVW_MODULE_PYTHON_API PythonExecutionOutputObeserver{
public:
    enum OutputType
    {
        standard,
        error
    };
    virtual void onPyhonExecutionOutput(std::string msg,OutputType outputType) = 0;
    PythonExecutionOutputObeserver();

    static void pyhonExecutionOutputEvent(std::string msg,OutputType outputType);
    static std::vector<PythonExecutionOutputObeserver*> observers_;
};

} // namespace

#endif // IVW_PYTHONEXECUTIONOUTPUTOBESERVER_MODULE_H
