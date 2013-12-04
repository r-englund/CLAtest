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

TransferFunctionProperty::TransferFunctionProperty(std::string identifier, std::string displayName, TransferFunction value, PropertyOwner::InvalidationLevel invalidationLevel, PropertySemantics::Type semantics )
: TemplateProperty<TransferFunction>(identifier, displayName, value, invalidationLevel, semantics), 
    maskProperty_("mask_", "Mask", 0, 255, 0, 255), zoomHorizontalProperty_("zoomHorizontal_", "Zoom", 0, 255, 0, 255), 
    zoomVerticalProperty_("zoomVertical_", "Zoom", 0.f, 1.f, 0.f, 1.f, 0.01f), bitRangeProperty_("bitRangeProperty", "Bit Range") 
{
    bitRangeProperty_.addOption("8", "8-bit", 8);
    bitRangeProperty_.addOption("12", "12-bit", 12);
    bitRangeProperty_.addOption("16", "16-bit", 16);
    bitRangeProperty_.setSelectedOption(0);
}

IntMinMaxProperty* TransferFunctionProperty::maskProperty(){
    return &maskProperty_;
}

IntMinMaxProperty* TransferFunctionProperty::zoomHorizontalProperty(){
    return &zoomHorizontalProperty_;
}

FloatMinMaxProperty* TransferFunctionProperty::zoomVerticalProperty(){
    return &zoomVerticalProperty_;
}

OptionPropertyInt* TransferFunctionProperty::bitRangeProperty(){
    return &bitRangeProperty_;
}

void TransferFunctionProperty::updateMask(ivec2 mask, int width){
    float propertyMaskMinPos = static_cast<float>(mask.x) / (float)width;
    float propertyMaskMaxPos = static_cast<float>(mask.y) / (float)width;

    propertyMaskMinPos = (propertyMaskMinPos < 0.f) ? 0.f : propertyMaskMinPos;
    propertyMaskMinPos = (propertyMaskMinPos > 1.f) ? 1.f : propertyMaskMinPos;

    propertyMaskMaxPos = (propertyMaskMaxPos < 0.f) ? 0.f : propertyMaskMaxPos;
    propertyMaskMaxPos = (propertyMaskMaxPos > 1.f) ? 1.f : propertyMaskMaxPos;

    get().setMaskMin(propertyMaskMinPos);
    get().setMaskMax(propertyMaskMaxPos);
}

void TransferFunctionProperty::serialize(IvwSerializer& s) const {
	Property::serialize(s);
	std::stringstream stream;
    s.serialize("size", (int)value_.getNumberOfDataPoints());
    s.serialize("mask_", maskProperty_);
    s.serialize("zoomHorizontal_", zoomHorizontalProperty_);
    s.serialize("zoomVertical_", zoomVerticalProperty_);
    s.serialize("bitRangeProperty_", bitRangeProperty_);

	for (int i = 0; i < static_cast<int>(value_.getNumberOfDataPoints()); i++){
		stream << "pos" << i;
		s.serialize(stream.str(), value_.getPoint(i)->getPos());
		stream.clear();
		stream.str(std::string());

		stream << "rgba" << i;
		s.serialize(stream.str(), value_.getPoint(i)->getRgba());
		stream.clear();
		stream.str(std::string());
	}
}

void TransferFunctionProperty::deserialize(IvwDeserializer& d) {
	Property::deserialize(d);
	int size;
	vec2 pos;
	vec4 rgba;
	std::stringstream stream;

	d.deserialize("size", size);
    d.deserialize("mask_", maskProperty_);
    d.deserialize("zoomHorizontal_", zoomHorizontalProperty_);
    d.deserialize("zoomVertical_", zoomVerticalProperty_);
    d.deserialize("bitRangeProperty_", bitRangeProperty_);

    updateMask(maskProperty_.get(), static_cast<int>(pow(2.f, bitRangeProperty_.get())));

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

    propertyModified();
}

} // namespace
