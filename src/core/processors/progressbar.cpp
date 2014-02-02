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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#include <inviwo/core/processors/progressbar.h>
#include <inviwo/core/io/serialization/ivwserialization.h>
#include <inviwo/core/util/assertion.h>

namespace inviwo {

ProgressBar::ProgressBar() :
    progress_(0.0f), beginLoopProgress_(-1.0f)
{}

ProgressBar::~ProgressBar() {}

void ProgressBar::updateProgress(float progress) {
    if (visible_) {
        ivwAssert(progress>=progress_, "Progress should always increase");
        ivwAssert(progress>=0.0f&&progress<=1.0, "Progress out of bounds.");
        progress_ = progress;
        notifyProgressChanged();
    }
}

void ProgressBar::updateProgressLoop(size_t loopVar, size_t maxLoopVar, float endLoopProgress) {
    if (visible_) {
        if (beginLoopProgress_<=0.0f)
            beginLoopProgress_ = progress_;

        float normalizedLoopVar = static_cast<float>(loopVar)/static_cast<float>(maxLoopVar);
        progress_ = beginLoopProgress_+normalizedLoopVar*(endLoopProgress-beginLoopProgress_);

        if (loopVar == maxLoopVar)
            beginLoopProgress_ = -1.0f;

        notifyProgressChanged();
    }
}

void ProgressBar::serialize(IvwSerializer& s) const {
    s.serialize("visible", visible_);
}

void ProgressBar::deserialize(IvwDeserializer& d) {
    d.deserialize("visible", visible_);
}

} // namespace
