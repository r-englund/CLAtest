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
 * Main file author: Hans-Christian Helltegen
 *
 *********************************************************************************/

#include <inviwo/core/interaction/trackballaction.h>

namespace inviwo {

TrackballAction::TrackballAction(TrackballAction::Actions action) {
    actionNames_[TRACKBALL_ROTATE] = "Trackball rotate";
    actionNames_[TRACKBALL_ZOOM] = "Trackball zoom";
    actionNames_[TRACKBALL_PAN] = "Trackball pan";
    actionNames_[TRACKBALL_STEPROTATE_UP] = "Step rotate up";
    actionNames_[TRACKBALL_STEPROTATE_LEFT] = "Step rotate left";
    actionNames_[TRACKBALL_STEPROTATE_DOWN] = "Step rotate down";
    actionNames_[TRACKBALL_STEPROTATE_RIGHT] = "Step rotate right";
    actionNames_[TRACKBALL_STEPZOOM_IN] = "Step zoom in";
    actionNames_[TRACKBALL_STEPZOOM_OUT] = "Step zoom out";
    actionNames_[TRACKBALL_STEPPAN_UP] = "Step pan up";
    actionNames_[TRACKBALL_STEPPAN_LEFT] = "Step pan left";
    actionNames_[TRACKBALL_STEPPAN_DOWN] = "Step pan down";
    actionNames_[TRACKBALL_STEPPAN_RIGHT] = "Step pan right";
    action_ = action;
    actionName_ = actionNames_[action];
}

TrackballAction::~TrackballAction() {}

void TrackballAction::serialize(IvwSerializer& s) const {
    Action::serialize(s);
    s.serialize("action", actionName_);
}
void TrackballAction::deserialize(IvwDeserializer& d) {
    d.deserialize("action", actionName_);

    for (int i = 0; i < COUNT; ++i) {
        if (actionNames_[i] == actionName_) {
            action_ = i;
            break;
        }
    }
}

} //namespace