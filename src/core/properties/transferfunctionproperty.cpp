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

#include <inviwo/core/properties/transferfunctionproperty.h>

namespace inviwo {

TransferFunctionProperty::TransferFunctionProperty(std::string identifier, std::string displayName, TransferFunction value,
                                                   VolumeInport* volumeInport, PropertyOwner::InvalidationLevel invalidationLevel, PropertySemantics semantics)
    : TemplateProperty<TransferFunction>(identifier, displayName, value, invalidationLevel, semantics)
    , volumeInport_(volumeInport)
    , mask_(0.0f, 1.0f)
    , zoomH_(0.0f, 1.0f)
    , zoomV_(0.0f, 1.0f)
{}

TransferFunctionProperty::~TransferFunctionProperty() {
    volumeInport_ = NULL;
    notifyObservers();
}

void TransferFunctionProperty::serialize(IvwSerializer& s) const {
	Property::serialize(s);
	std::stringstream stream;
    s.serialize("size", (int)value_.getNumDataPoints());
	for (size_t i=0; i<value_.getNumDataPoints(); i++){
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
}

void TransferFunctionProperty::deserialize(IvwDeserializer& d) {
	Property::deserialize(d);
	int size;
	vec2 pos;
	vec4 rgba;
	std::stringstream stream;

	d.deserialize("size", size);
    for (int i = 0; i < size; i++){
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
    propertyModified();
}

VolumeInport* TransferFunctionProperty::getVolumeInport(){
    return volumeInport_;
}

} // namespace
