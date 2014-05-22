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
 * Main file authors: Viktor Axelsson, Timo Ropinski, Erik Sundén
 *
 *********************************************************************************/

#include <inviwo/core/properties/transferfunctionproperty.h>

namespace inviwo {

TransferFunctionProperty::TransferFunctionProperty(std::string identifier,
                                                   std::string displayName,
                                                   TransferFunction value,
                                                   VolumeInport* volumeInport,
                                                   PropertyOwner::InvalidationLevel invalidationLevel,
                                                   PropertySemantics semantics)
    : TemplateProperty<TransferFunction>(identifier, displayName, value, invalidationLevel, semantics)
    , zoomH_(0.0f, 1.0f)
    , defaultZoomH_(0.0f, 1.0f)
    , zoomV_(0.0f, 1.0f)
    , defaultZoomV_(0.0f, 1.0f)
    , showHistogram_(true)
    , defaultShowHistogram_(true)
    , volumeInport_(volumeInport) {
}

TransferFunctionProperty::~TransferFunctionProperty() {
    volumeInport_ = NULL;
}

void TransferFunctionProperty::setShowHistogram(bool show) {
    showHistogram_ = show;
}

bool TransferFunctionProperty::getShowHistogram() {
    return showHistogram_;
}

VolumeInport* TransferFunctionProperty::getVolumeInport() {
    return volumeInport_;
}

void TransferFunctionProperty::resetToDefaultState() {
    zoomH_ = defaultZoomH_;
    zoomV_ = defaultZoomV_;
    showHistogram_ = defaultShowHistogram_;
    TemplateProperty<TransferFunction>::resetToDefaultState();
}
void TransferFunctionProperty::setCurrentStateAsDefault() {
    TemplateProperty<TransferFunction>::setCurrentStateAsDefault();
    defaultZoomH_ = zoomH_;
    defaultZoomV_ = zoomV_;
    defaultShowHistogram_ = showHistogram_;
}

void TransferFunctionProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s);
    if (zoomH_ != defaultZoomH_) {
        s.serialize("zoomH_", zoomH_);
    }
    if (zoomV_ != defaultZoomV_) {
        s.serialize("zoomV_", zoomV_);
    }
    if (showHistogram_ != defaultShowHistogram_) {
        s.serialize("showHistogram_", showHistogram_);
    }
    if (this->value_ != this->defaultValue_) {
        s.serialize("transferFunction", this->value_);
    }
}

void TransferFunctionProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d);
    d.deserialize("zoomH_", zoomH_);
    d.deserialize("zoomV_", zoomV_);
    d.deserialize("showHistogram_", showHistogram_);
    d.deserialize("transferFunction", this->value_);
    propertyModified();
}

void TransferFunctionProperty::setMask(float maskMin, float maskMax) {
    if (maskMax < maskMin) {
        maskMax = maskMin;
    }
    this->value_.setMaskMin(maskMin);
    this->value_.setMaskMax(maskMax);
}

const vec2 TransferFunctionProperty::getMask() const {
    return vec2(this->value_.getMaskMin(), this->value_.getMaskMax());
}

const vec2& TransferFunctionProperty::getZoomH() const {
    return zoomH_;
}

void TransferFunctionProperty::setZoomH(float zoomHMin, float zoomHMax) {
    if (zoomHMax < zoomHMin) {
        zoomHMax = zoomHMin;
    }
    zoomH_ = vec2(zoomHMin, zoomHMax);
}

const vec2& TransferFunctionProperty::getZoomV() const {
    return zoomV_;
}

void TransferFunctionProperty::setZoomV(float zoomVMin, float zoomVMax) {
    if (zoomVMax < zoomVMin) {
        zoomVMax = zoomVMin;
    }
    zoomV_ = vec2(zoomVMin, zoomVMax);
}


} // namespace
