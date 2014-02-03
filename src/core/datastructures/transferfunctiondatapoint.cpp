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

TransferFunctionDataPoint::TransferFunctionDataPoint(TransferFunction* transFunc, const vec2& pos, const vec4& rgba)
    : pos_(pos)
    , rgba_(rgba)
    , transFunc_(transFunc) {
}

TransferFunctionDataPoint::~TransferFunctionDataPoint() {}

const vec2& TransferFunctionDataPoint::getPos() const {
    return pos_;
}

void TransferFunctionDataPoint::setPos(const vec2& pos) {
    pos_ = pos;
    transFunc_->calcTransferValues();
}

const vec4& TransferFunctionDataPoint::getRGBA() const {
    return rgba_;
}

void TransferFunctionDataPoint::setRGBA(const vec4& rgba) {
    rgba_ = rgba;
    transFunc_->calcTransferValues();
}

void TransferFunctionDataPoint::setRGB(const vec3& rgb) {
    rgba_.r = rgb.r;
    rgba_.g = rgb.g;
    rgba_.b = rgb.b;
    transFunc_->calcTransferValues();
}

void TransferFunctionDataPoint::setA(float alpha) {
    rgba_.a = alpha;
    transFunc_->calcTransferValues();
}

void TransferFunctionDataPoint::setPosA(const vec2& pos, float alpha) {
    pos_ = pos;
    rgba_.a = alpha;
    transFunc_->calcTransferValues();
}

} // namespace