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
    , mask_(0.0f, 1.0f)
    , zoomH_(0.0f, 1.0f)
    , zoomV_(0.0f, 1.0f)
    , showHistogram_(true)
    , volumeInport_(volumeInport) {
}

TransferFunctionProperty::~TransferFunctionProperty() {
    volumeInport_ = NULL;
    notifyObservers();
}

void TransferFunctionProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s);
    std::stringstream stream;
    s.serialize("size", (int)value_.getNumDataPoints());

    for (size_t i=0; i<value_.getNumDataPoints(); i++) {
        stream << "pos" << i;
        s.serialize(stream.str(), value_.getPoint(static_cast<int>(i))->getPos());
        stream.clear();
        stream.str(std::string());
        stream << "rgba" << i;
        s.serialize(stream.str(), value_.getPoint(static_cast<int>(i))->getRGBA());
        stream.clear();
        stream.str(std::string());
    }

    s.serialize("mask_", mask_);
    s.serialize("zoomH_", zoomH_);
    s.serialize("zoomV_", zoomV_);
    TransferFunction::InterpolationType interpolationType = value_.getInterpolationType();
    s.serialize("interpolationType_", interpolationType);
    s.serialize("showHistogram_", showHistogram_);
}

void TransferFunctionProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d);
    int size;
    vec2 pos;
    vec4 rgba;
    std::stringstream stream;
    d.deserialize("size", size);
    value_.clearPoints();

    for (int i = 0; i < size; i++) {
        stream << "pos" << i;
        d.deserialize(stream.str(), pos);
        stream.clear();
        stream.str(std::string());
        stream << "rgba" << i;
        d.deserialize(stream.str(), rgba);
        stream.clear();
        stream.str(std::string());
        value_.addPoint(pos, rgba);
    }

    d.deserialize("mask_", mask_);
    get().setMaskMin(mask_.x);
    get().setMaskMax(mask_.y);
    d.deserialize("zoomH_", zoomH_);
    d.deserialize("zoomV_", zoomV_);
    int interpolationType;
    d.deserialize("interpolationType_", interpolationType);
    get().setInterpolationType(static_cast<TransferFunction::InterpolationType>(interpolationType));
    d.deserialize("showHistogram_", showHistogram_);
    propertyModified();
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

} // namespace
