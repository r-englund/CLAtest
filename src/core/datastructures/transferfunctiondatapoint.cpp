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
 * Main file author: Timo Ropinski
 *
 *********************************************************************************/

#include <inviwo/core/datastructures/transferfunction.h>
#include <inviwo/core/datastructures/transferfunctiondatapoint.h>

namespace inviwo {

TransferFunctionDataPoint::TransferFunctionDataPoint(const vec2& pos, const vec4& rgba)
    : pos_(pos)
    , rgba_(rgba) {
}

TransferFunctionDataPoint::~TransferFunctionDataPoint() {}

void TransferFunctionDataPoint::setPos(const vec2& pos) {
    pos_ = pos;
    notifyTransferFunctionPointObservers();
}

void TransferFunctionDataPoint::setRGBA(const vec4& rgba) {
    rgba_ = rgba;
    notifyTransferFunctionPointObservers();
}

void TransferFunctionDataPoint::setRGB(const vec3& rgb) {
    rgba_.r = rgb.r;
    rgba_.g = rgb.g;
    rgba_.b = rgb.b;
    notifyTransferFunctionPointObservers();
}

void TransferFunctionDataPoint::setA(float alpha) {
    pos_.y = alpha;
    rgba_.a = alpha;
    notifyTransferFunctionPointObservers();
}

void TransferFunctionDataPoint::setPosA(const vec2& pos, float alpha) {
    pos_ = pos;
    rgba_.a = alpha;
    notifyTransferFunctionPointObservers();
}

void TransferFunctionDataPoint::notifyTransferFunctionPointObservers() const {
    // Notify observers
    for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it) {
        // static_cast can be used since only template class objects can be added
        static_cast<TransferFunctionPointObserver*>(*it)->onTransferFunctionPointChange(this);
    }
}

void TransferFunctionDataPoint::serialize(IvwSerializer& s) const {
    s.serialize("pos", pos_);
    s.serialize("rgba", rgba_);
}

void TransferFunctionDataPoint::deserialize(IvwDeserializer& d) {
    d.deserialize("pos", pos_);
    d.deserialize("rgba", rgba_);
}

} // namespace