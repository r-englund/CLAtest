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

#include <inviwo/core/interaction/pickingcontainer.h>
#include <inviwo/core/interaction/pickingmanager.h>
#include <inviwo/core/datastructures/image/image.h>
#include <inviwo/core/datastructures/image/layerram.h>

namespace inviwo {

PickingContainer::PickingContainer()
    : src_(NULL)
    , currentPickObj_(NULL)
    , prevCoord_(uvec2(0,0))
    , selected_(false) {};

PickingContainer::~PickingContainer() {};

bool PickingContainer::isPickableSelected() { return selected_; }

bool PickingContainer::performPick(const uvec2& coord) {
    prevCoord_ = coord;

    if (src_) {
        const LayerRAM* pickingRAM = src_->getPickingLayer()->getRepresentation<LayerRAM>();
        vec4 value = pickingRAM->getValueAsVec4Float(coord);
        vec3 pickedColor = (value.a > 0.f ? value.rgb() : vec3(0.f));
        DataVec3UINT8::type pickedColorUINT8;
        DataVec3UINT8::get()->vec3ToValue(pickedColor*255.f, &pickedColorUINT8);
        currentPickObj_ = PickingManager::instance()->getPickingObjectFromColor(pickedColorUINT8);

        if (currentPickObj_) {
            setPickableSelected(true);
            currentPickObj_->setPickingPosition(normalizedCoordinates(coord));

            if (currentPickObj_->readDepth()) {
                const LayerRAM* depthRAM = src_->getDepthLayer()->getRepresentation<LayerRAM>();
                float depth = depthRAM->getValueAsSingleFloat(coord);
                currentPickObj_->setPickingDepth(depth);
            }

            currentPickObj_->setPickingMove(vec2(0.f,0.f));
            currentPickObj_->picked();
            return true;
        }
    }

    setPickableSelected(false);
    return false;
}

void PickingContainer::movePicked(const uvec2& coord) {
    currentPickObj_->setPickingMove(pixelMoveVector(prevCoord_, coord));
    prevCoord_ = coord;
    currentPickObj_->picked();
}

void PickingContainer::setPickableSelected(bool selected) {
    selected_ = selected;
}

void PickingContainer::setPickingSource(const Image* src) {
    src_ = src;
}

vec2 PickingContainer::pixelMoveVector(const uvec2& previous, const uvec2& current) {
    return vec2((static_cast<float>(current.x)-static_cast<float>(previous.x))/static_cast<float>(src_->getDimension().x),
                (static_cast<float>(current.y)-static_cast<float>(previous.y))/static_cast<float>(src_->getDimension().y));
}

vec2 PickingContainer::normalizedCoordinates(const uvec2& coord) {
    return vec2(static_cast<float>(coord.x)/static_cast<float>(src_->getDimension().x),
                static_cast<float>(coord.y)/static_cast<float>(src_->getDimension().y));
}

} // namespace