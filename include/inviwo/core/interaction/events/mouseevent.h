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

#ifndef IVW_MOUSEEVENT_H
#define IVW_MOUSEEVENT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/interaction/events/interactionevent.h>

namespace inviwo {

class IVW_CORE_API MouseEvent : public InteractionEvent {
public:
    enum MouseButton {
        MOUSE_BUTTON_NONE   =      0,
        MOUSE_BUTTON_LEFT,
        MOUSE_BUTTON_MIDDLE,
        MOUSE_BUTTON_RIGHT,
        COUNT
    };

    enum MouseState {
        MOUSE_STATE_NONE    =      0,
        MOUSE_STATE_MOVE,
        MOUSE_STATE_PRESS,
        MOUSE_STATE_RELEASE
    };

    MouseEvent(ivec2 position, MouseEvent::MouseButton button,
               MouseEvent::MouseState state, InteractionEvent::Modifier modifier, uvec2 canvasSize);
    MouseEvent(MouseEvent::MouseButton button, InteractionEvent::Modifier modifier);
    ~MouseEvent();

    inline ivec2 pos() const { return position_; }
    inline vec2 posNormalized() const { return vec2(vec2(position_)/vec2(canvasSize_)); }
    inline unsigned int x() const { return position_.x; }
    inline unsigned int y() const { return position_.y; }
    inline MouseEvent::MouseState state() const { return state_; }
    inline uvec2 canvasSize() const {return canvasSize_; }

    virtual std::string getClassName() const { return "MouseEvent"; }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

private:
    ivec2 position_;
    MouseEvent::MouseState state_;
    uvec2 canvasSize_;
    std::string buttonNames_[COUNT];
};

} // namespace

#endif // IVW_MOUSEEVENT_H