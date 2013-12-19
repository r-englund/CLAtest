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

#ifndef IVW_MODULE_ACTION_H
#define IVW_MODULE_ACTION_H

#include <inviwo/core/common/modulecallback.h>

namespace inviwo {

class IVW_CORE_API ModuleCallBackActionState {
public:
    enum Status { Default =0,
                  Enabled =1,
                  Disabled =2,
                  Custom =3
                };
};

//TODO: several types of call back action required ???
class IVW_CORE_API ModuleCallbackAction {
public:
    ModuleCallbackAction(std::string actionName, InviwoModule* module, ModuleCallBackActionState::Status state=ModuleCallBackActionState::Disabled);    
    std::string getActionName();
    InviwoModule* getModule();
    ModuleCallback* getCallBack();    
    void setActionState(ModuleCallBackActionState::Status state);
    ModuleCallBackActionState::Status getActionState();    
private:
    InviwoModule* module_;
    std::string actionName_;
    // TODO: for now call backs with single argument is supported
    ModuleCallback callBack_;
    ModuleCallBackActionState::Status actionState_;
};

} // namespace

#endif // IVW_MODULE_ACTION_H
