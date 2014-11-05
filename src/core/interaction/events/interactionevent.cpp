/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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

#include <inviwo/core/interaction/events/interactionevent.h>

namespace inviwo {

const std::string InteractionEvent::modifierNames_[] = {"", "Alt", "Ctrl", "Shift"};

InteractionEvent::InteractionEvent() {}

InteractionEvent::InteractionEvent(const InteractionEvent& rhs)
    : Event(rhs)
    , modifier_(rhs.modifier_)
    , button_(rhs.button_)
    , modifierName_(rhs.modifierName_)
    , buttonName_(rhs.buttonName_) {
}

InteractionEvent& InteractionEvent::operator=(const InteractionEvent& that) {
    if (this != &that) {
        Event::operator=(that);
        modifier_ = that.modifier_;
        button_ = that.button_;
        modifierName_ = that.modifierName_;
        buttonName_ = that.buttonName_;
    }
    return *this;
}

InteractionEvent* InteractionEvent::clone() const {
    return new InteractionEvent(*this);
}

InteractionEvent::~InteractionEvent() {}

void InteractionEvent::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassIdentifier(), true);
    s.serialize("modifier", modifierName_);
}

void InteractionEvent::deserialize(IvwDeserializer& d) {
    std::string className;
    d.deserialize("type", className, true);
    d.deserialize("modifier", modifierName_);

    for (size_t i = 0; i < COUNT; ++i) {
        if (modifierNames_[i] == modifierName_) {
            modifier_ = static_cast<InteractionEvent::Modifier>(i);
            break;
        }
    }
}

} // namespace