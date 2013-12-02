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

#include <modules/python/pythonmodule.h>

#include "pythonexecutionoutputobeserver.h"

namespace inviwo {



PythonExecutionOutputObeserver::PythonExecutionOutputObeserver(){
    observers_.push_back(this);
}

void PythonExecutionOutputObeserver::pyhonExecutionOutputEvent(std::string msg,OutputType outputType){
    std::vector<PythonExecutionOutputObeserver*>::iterator it;
    for(it = observers_.begin();it != observers_.end();++it){
        (*it)->onPyhonExecutionOutput(msg,outputType);
    }
}

std::vector<PythonExecutionOutputObeserver*> PythonExecutionOutputObeserver::observers_;


} // namespace
