/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_EVENTHANDLER_H
#define IVW_EVENTHANDLER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/events/event.h>
#include <inviwo/core/interaction/events/eventlistener.h>

namespace inviwo {

class IVW_CORE_API EventHandler {

public:
    EventHandler();
    virtual ~EventHandler();

    void broadcast(Event* event);

protected:
    bool addEventListener(EventListener*);
    bool removeEventListener(EventListener*);

private:
    std::vector<EventListener*> eventListeners_;        
};

} // namespace

#endif // IVW_EVENTHANDLER_H