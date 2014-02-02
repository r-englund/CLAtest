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

#ifndef IVW_TIMEREVENT_H
#define IVW_TIMEREVENT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/events/event.h>

namespace inviwo {

class IVW_CORE_API TimerEvent : public Event {

public:
    TimerEvent();
    virtual ~TimerEvent();
};

} // namespace

#endif // IVW_TIMEREVENT_H