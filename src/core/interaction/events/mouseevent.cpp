/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Contact: Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/core/interaction/events/mouseevent.h>

namespace inviwo {

MouseEvent::MouseEvent(ivec2 position, int delta, MouseEvent::MouseButton button,
                       MouseEvent::MouseState state, MouseEvent::MouseWheelOrientation orientation,
                       InteractionEvent::Modifier modifiers, uvec2 canvasSize)
    : InteractionEvent(modifiers)
    , button_(button)
    , state_(state)
    , wheelOrientation_(orientation)

    , position_(position)
    , wheelSteps_(delta)
    , canvasSize_(canvasSize) {}

MouseEvent::MouseEvent(ivec2 position, MouseEvent::MouseButton button,
                       MouseEvent::MouseState state /*= MOUSE_STATE_NONE*/,
                       InteractionEvent::Modifier modifiers /*= InteractionEvent::MODIFIER_NONE*/,
                       uvec2 canvasSize /*= uvec2(0)*/)
    : InteractionEvent(modifiers)
    , button_(button)
    , state_(state)
    , wheelOrientation_(MOUSE_WHEEL_NONE)

    , position_(position)
    , wheelSteps_(0)
    , canvasSize_(canvasSize) {}

MouseEvent::MouseEvent(MouseEvent::MouseButton button,
                       InteractionEvent::Modifier modifiers /*= InteractionEvent::MODIFIER_NONE*/,
                       MouseEvent::MouseState state /*= MOUSE_STATE_NONE*/,
                       MouseEvent::MouseWheelOrientation orientation /*= MOUSE_WHEEL_NONE*/)
    : InteractionEvent(modifiers)
    , button_(button)
    , state_(state)
    , wheelOrientation_(orientation)

    , position_(0)
    , wheelSteps_(0)
    , canvasSize_(0) {}

MouseEvent::~MouseEvent() {}

void MouseEvent::modify(ivec2 newPosition, uvec2 newCanvasSize) {
    position_ = newPosition;
    canvasSize_ = newCanvasSize;
};

void MouseEvent::serialize(IvwSerializer& s) const {
    InteractionEvent::serialize(s);
    s.serialize("button", button_);
    s.serialize("state", state_);
    s.serialize("wheelOrientation", wheelOrientation_);
}

void MouseEvent::deserialize(IvwDeserializer& d) {
    InteractionEvent::deserialize(d);

    int button = button_;
    d.deserialize("button", button);
    button_ = static_cast<MouseButton>(button);

    int state = state_;
    d.deserialize("state", state);
    state_ = static_cast<MouseState>(state);

    int wheelOrientation = wheelOrientation_;
    d.deserialize("wheelOrientation", wheelOrientation);
    wheelOrientation_ = static_cast<MouseWheelOrientation>(wheelOrientation);
}

const std::string MouseEvent::buttonNames_[COUNT] = {"", "Left button", "Right button",
                                                     "Right button"};

std::string MouseEvent::getClassIdentifier() const { return "org.inviwo.MouseEvent"; }

bool MouseEvent::matching(const Event* aEvent) const {
    const MouseEvent* event = dynamic_cast<const MouseEvent*>(aEvent);
    if (event) {
        return matching(event);
    } else {
        return false;
    }
}
bool MouseEvent::matching(const MouseEvent* aEvent) const {
    return button_ == aEvent->button_ && state_ == aEvent->state_ &&
           wheelOrientation_ == aEvent->wheelOrientation_ && modifiers_ == aEvent->modifiers_;
}

}  // namespace