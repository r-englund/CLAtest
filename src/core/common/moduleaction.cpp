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

#include <inviwo/core/common/moduleaction.h>

namespace inviwo {


ModuleCallbackAction::ModuleCallbackAction(std::string actionName, InviwoModule* module, ModuleCallBackActionState::Status state) 
                                            : actionName_(actionName)
                                              , module_(module)
                                              , actionState_(state) {
}

std::string ModuleCallbackAction::getActionName() {
    return actionName_;
}

InviwoModule* ModuleCallbackAction::getModule() {
    return module_;
}

ModuleCallback* ModuleCallbackAction::getCallBack() {
    return &callBack_;
}

ModuleCallBackActionState::Status ModuleCallbackAction::getActionState() {
    return actionState_;
}

void ModuleCallbackAction::setActionState(ModuleCallBackActionState::Status state) {
    actionState_ = state;
}



} // namespace

