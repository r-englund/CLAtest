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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <modules/pythonqt/pythonqtmodule.h>

#include <modules/python/pythoninterface/pymodule.h>
#include "pythonqtmethods/pythonqtmethods.h"
#include "pythonmenu.h"

namespace inviwo {

PythonQtModule::PythonQtModule() : InviwoModule() {
    setIdentifier("PythonQt");
    initPyQtModule();

    menu_ = new PythonMenu();
}

PythonQtModule::~PythonQtModule(){
    delete inviwoPyQtModule_;
    delete menu_;
}

void PythonQtModule::initPyQtModule(){
    inviwoPyQtModule_ = new PyModule("inviwoqt");
    inviwoPyQtModule_->addMethod(new PyLoadNetwork());
}

} // namespace
