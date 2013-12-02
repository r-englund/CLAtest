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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_RESIZEEVENT_H
#define IVW_RESIZEEVENT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/interaction/events/event.h>

namespace inviwo {

    class IVW_CORE_API ResizeEvent : public Event {

    public:        
        ResizeEvent(uvec2 newSize);
        ~ResizeEvent();
        inline uvec2 size() const {return size_; }
        inline uvec2 previousSize() const {return previousSize_; }
        inline void setSize(uvec2 csize) {size_ = csize;} 
        inline void setPreviousSize(uvec2 previousSize) {previousSize_ = previousSize;}     

    private:        
        uvec2 size_;
        uvec2 previousSize_;
    };

} // namespace

#endif // IVW_RESIZEEVENT_H