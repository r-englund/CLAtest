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
 * Primary author : Viktor Axelsson
 *
 **********************************************************************/

#include <inviwo/core/datastructures/transferfunction.h>
#include <inviwo/core/datastructures/transferfunctiondatapoint.h>

namespace inviwo {

TransferFunctionDataPoint::TransferFunctionDataPoint(TransferFunction* transFunc, const vec2& pos, const vec4& rgba)
    : transFunc_(transFunc)
    , pos_(pos)
    , rgba_(rgba)
{}

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