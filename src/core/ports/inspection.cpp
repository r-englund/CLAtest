 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 * Main file author: Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/core/ports/inspection.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/settings/systemsettings.h>

namespace inviwo {

Inspection::Inspection()
    : processorIdentifier_("")
    , portIdentifier_("")
    , state_(Start)
    , active_(NULL)
    , size_(NULL) {
}

Inspection::~Inspection() {
}

bool Inspection::isActive() {
    if (!active_) {
        if (InviwoApplication::getPtr()) {
            active_ = dynamic_cast<BoolProperty*>(
                          InviwoApplication::getPtr()->
                          getSettingsByType<SystemSettings>()->
                          getPropertyByIdentifier("enablePortInspectors"));
        } else
            return false;
    }

    return active_->get();
}

int Inspection::size() {
    if (!size_) {
        if (InviwoApplication::getPtr()) {
            size_ = dynamic_cast<IntProperty*>(
                        InviwoApplication::getPtr()->
                        getSettingsByType<SystemSettings>()->
                        getPropertyByIdentifier("portInspectorSize"));
        } else
            return 128;
    }

    return size_->get();
}

inviwo::Inspection::State Inspection::state() const {
    return state_;
}

void Inspection::setState(State val) {
    state_ = val;
}

void Inspection::setPort(Port* port) {
    processorIdentifier_ = port->getProcessor()->getIdentifier();
    portIdentifier_ = port->getIdentifier();
}

bool Inspection::samePort(Port* port) {
    if (processorIdentifier_ == port->getProcessor()->getIdentifier()
        && portIdentifier_ == port->getIdentifier())
        return true;
    else
        return false;
}

void Inspection::resetPort() {
    processorIdentifier_ = "";
    portIdentifier_ = "";
}

} // namespace

