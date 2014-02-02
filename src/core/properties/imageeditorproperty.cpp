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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/properties/imageeditorproperty.h>

namespace inviwo {

ImageLabel::ImageLabel() : name_(""), startPoint_(0), rectSize_(0) {
}

ImageLabel::ImageLabel(vec2 startPoint, vec2 rectSize, std::string name)
    : name_(name)
    , startPoint_(startPoint)
    , rectSize_(rectSize) {
}

void ImageLabel::serialize(IvwSerializer& s) const {
    s.serialize("labelName", name_, true);
    s.serialize("topLeft", startPoint_);
    s.serialize("size", rectSize_);
}

void ImageLabel::deserialize(IvwDeserializer& d) {
    d.deserialize("labelName", name_, true);
    d.deserialize("topLeft", startPoint_);
    d.deserialize("size", rectSize_);
}


ImageEditorProperty::ImageEditorProperty(std::string identifier, std::string displayName,std::string value,
        PropertyOwner::InvalidationLevel invalidationLevel,
        PropertySemantics semantics)
    : FileProperty(identifier, displayName, value, invalidationLevel, semantics)
{}

void ImageEditorProperty::setDimension(ivec2 imgSize) {
    dimensions_ = imgSize;
}

void ImageEditorProperty::addLabel(vec2 startPoint, vec2 rectSize, std::string name) {
    labels_.push_back(new ImageLabel(startPoint, rectSize, name));
}

std::vector<ImageLabel*> ImageEditorProperty::getLabels() const {
    return labels_;
}

void ImageEditorProperty::clearLabels() {
    for (size_t i=0; i<labels_.size(); i++)
        delete labels_[i];

    labels_.clear();
}

void ImageEditorProperty::serialize(IvwSerializer& s) const {
    FileProperty::serialize(s);
    s.serialize("Dimension", dimensions_);
    s.serialize("Labels", labels_, "Label");
}

void ImageEditorProperty::deserialize(IvwDeserializer& d) {
    FileProperty::deserialize(d);
    d.deserialize("Dimension", dimensions_);
    d.deserialize("Labels", labels_, "Label");
}


} // namespace
