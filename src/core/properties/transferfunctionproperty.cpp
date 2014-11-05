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
 * Contact: Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/core/properties/transferfunctionproperty.h>

namespace inviwo {

PropertyClassIdentifier(TransferFunctionProperty, "org.inviwo.TransferFunctionProperty");

TransferFunctionProperty::TransferFunctionProperty(const std::string &identifier,
                                                   const std::string &displayName,
                                                   const TransferFunction &value,
                                                   VolumeInport* volumeInport,
                                                   PropertyOwner::InvalidationLevel invalidationLevel,
                                                   PropertySemantics semantics)
    : TemplateProperty<TransferFunction>(identifier, displayName, value, invalidationLevel, semantics)
    , TransferFunctionObserver()
    , zoomH_(0.0f, 1.0f)
    , defaultZoomH_(0.0f, 1.0f)
    , zoomV_(0.0f, 1.0f)
    , defaultZoomV_(0.0f, 1.0f)
    , showHistogram_(true)
    , defaultShowHistogram_(true)
    , volumeInport_(volumeInport) {
    
    this->value_.addObserver(this);
}

TransferFunctionProperty::TransferFunctionProperty(const TransferFunctionProperty& rhs)
    : TemplateProperty<TransferFunction>(rhs)
    , TransferFunctionObserver()
    , zoomH_(rhs.zoomH_)
    , defaultZoomH_(rhs.defaultZoomH_)
    , zoomV_(rhs.zoomV_)
    , defaultZoomV_(rhs.defaultZoomV_)
    , showHistogram_(rhs.showHistogram_)
    , defaultShowHistogram_(rhs.defaultShowHistogram_)
    , volumeInport_(rhs.volumeInport_) {

    this->value_.addObserver(this);

}

TransferFunctionProperty& TransferFunctionProperty::operator=(const TransferFunctionProperty& that) {
    if (this != &that) {
        this->value_.removeObserver(this);
        TemplateProperty<TransferFunction>::operator=(that);
        this->value_.addObserver(this);
        zoomH_ = that.zoomH_;
        defaultZoomH_ = that.defaultZoomH_;
        zoomV_ = that.zoomV_;
        defaultZoomV_ = that.defaultZoomV_;
        showHistogram_ = that.showHistogram_;
        defaultShowHistogram_ = that.defaultShowHistogram_;
        volumeInport_ = that.volumeInport_;
    }
    return *this;
}

TransferFunctionProperty* TransferFunctionProperty::clone() const {
    return new TransferFunctionProperty(*this);
}

TransferFunctionProperty::~TransferFunctionProperty() {
    volumeInport_ = NULL;
}

void TransferFunctionProperty::setShowHistogram(int type) {
    showHistogram_ = type;
}

int TransferFunctionProperty::getShowHistogram() {
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
    propertyModified();
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

void TransferFunctionProperty::set(const TransferFunction& value) {
    this->value_.removeObserver(this);
    TemplateProperty<TransferFunction>::set(value);
    this->value_.addObserver(this);
}

void TransferFunctionProperty::set(const Property *property) {
    const TransferFunctionProperty *tfp = dynamic_cast<const TransferFunctionProperty*>(property);
    if (tfp){
        set(tfp->get());
    }
    TemplateProperty<TransferFunction>::set(property);
}

void TransferFunctionProperty::onControlPointAdded(TransferFunctionDataPoint* p) {
    propertyModified();
}
void TransferFunctionProperty::onControlPointRemoved(TransferFunctionDataPoint* p) {
    propertyModified();
}
void TransferFunctionProperty::onControlPointChanged(const TransferFunctionDataPoint* p) {
    propertyModified();
}


} // namespace
