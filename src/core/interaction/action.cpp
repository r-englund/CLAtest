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

#include <inviwo/core/interaction/action.h>

namespace inviwo {

Action::Action(std::string name /*= ""*/) 
    : name_(name)
    , callback_(NULL) {
}

Action::Action(const Action& rhs)
    : name_(rhs.name_)
    , callback_(rhs.callback_) {
}

Action& Action::operator=(const Action& that) {
    if (this != &that) {
        name_ = that.name_;
        if(callback_) delete callback_;
        callback_ = that.callback_;
    }
    return *this;
}

Action* Action::clone() const {
    return new Action(*this);
}

Action::~Action() {
    if (callback_) delete callback_;
}

void Action::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassIdentifier(), true);
}

void Action::deserialize(IvwDeserializer& d) {
}

std::string Action::name() const {
    return name_;
}


std::string Action::getClassIdentifier() const {
    return "org.inviwo.Action";
}

void Action::invoke(Event* event) {
    if(callback_) {
        callback_->invoke(event);
    }
}

} // namespace