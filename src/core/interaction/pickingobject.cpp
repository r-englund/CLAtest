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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/core/interaction/pickingobject.h>

namespace inviwo {

PickingObject::PickingObject(size_t id, DataVec3UINT8::type c) : id_(id), colorUINT8_(c),
    pos_(vec2(0.f)), readDepth_(true), depth_(0.f), move_(vec2(0.f)) {
    onPickedCallback_ = new PickingCallback();
    color_ = DataVec3UINT8::get()->valueToNormalizedVec3Float(&c);
}

PickingObject::~PickingObject() {
    delete onPickedCallback_;
}

const size_t& PickingObject::getPickingId() const {
    return id_;
}

const vec3& PickingObject::getPickingColor() const {
    return color_;
}

const DataVec3UINT8::type& PickingObject::getPickingColorAsUINT8() const {
    return colorUINT8_;
}

const vec2& PickingObject::getPickingPosition() const {
    return pos_;
}

const vec2& PickingObject::getPickingMove() const {
    return move_;
}

void PickingObject::setReadDepth(bool rd) {
    readDepth_ = rd;
}

bool PickingObject::readDepth() {
    return readDepth_;
}

const float& PickingObject::getPickingDepth() const {
    return depth_;
}

void PickingObject::picked() const {
    onPickedCallback_->invoke(this);
}

void PickingObject::setPickingPosition(vec2 pos) {
    pos_ = pos;
}

void PickingObject::setPickingMove(vec2 move) {
    move_ = move;
}

void PickingObject::setPickingDepth(float depth) {
    depth_ = depth;
}

PickingCallback* PickingObject::getCallbackContainer() {
    return onPickedCallback_;
}

} // namespace
