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

#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo {

TransferFunctionProperty::TransferFunctionProperty(std::string identifier, std::string displayName, TransferFunction value,
                                                   PropertyOwner::InvalidationLevel invalidationLevel, PropertySemantics::Type semantics)
    : TemplateProperty<TransferFunction>(identifier, displayName, value, invalidationLevel, semantics)
    , mask_(0.0f, 1.0f)
    , zoomH_(0.0f, 1.0f)
    , zoomV_(0.0f, 1.0f)
{}

TransferFunctionProperty::~TransferFunctionProperty() {}

void TransferFunctionProperty::setVolume(const Volume* volume) {
    if (volume != volume_) {
        volume_ = volume;
        uvec3 dim = volume->getDimension();

        // allocate histogram
        std::vector<unsigned int> histogram;
        // FIXME: obtain bitdepth from volume
        unsigned int numValues = 256;//volume->getDataFormat();
        for (unsigned int i=0; i<numValues; i++) {
            histogram.push_back(0);
            histogram_.push_back(0.0f);
        }

        // fill histogram
        // TODO: Implement this without getValueAsSingleFloat,
        // should be done with a volumeoperation or something.
        // since getValueAsSingleFloat is very slow
        const VolumeRAM* volumeRAM = volume_->getRepresentation<VolumeRAM>();
        for (unsigned int x=0; x<dim.x; x++) {
            for (unsigned int y=0; y<dim.y; y++) {
                for (unsigned int z=0; z<dim.z; z++) {
                    float intensity = volumeRAM->getValueAsSingleFloat(uvec3(x,y,z));
                    // Temporary fix for intensity values outside of 0,1. needed for for float types where
                    // the values are not normalized. 
                    intensity = std::min(std::max(0.0f, intensity), 1.0f);
                    histogram[static_cast<int>(intensity*(numValues-1))]++;
                }
            }
        }

        // normalize histogram while excluding 0 values
        unsigned int maxOccurance = 0;
        for (unsigned int i=1; i<numValues; i++)
            if (histogram[i]>maxOccurance) maxOccurance = histogram[i];
        for (unsigned int i=1; i<numValues; i++)
            histogram_[i] = static_cast<float>(histogram[i])/static_cast<float>(maxOccurance);
        histogram_[0] = 1.0f;
        notifyObservers();
    }
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

} // namespace
