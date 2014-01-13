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
#include <inviwo/core/util/stringconversion.h>
#include "pyinviwo.h"

namespace inviwo {

PythonModule::PythonModule() : InviwoModule() {
    setIdentifier("Python");
    setXMLFileName("python/pythonmodule.xml");   

    pyInviwo_ = new PyInviwo();
}

PythonModule::~PythonModule(){
    delete pyInviwo_;
}

} // namespace
