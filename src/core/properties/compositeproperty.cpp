/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
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

#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {

CompositeProperty::CompositeProperty(std::string identifier, std::string displayName,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics semantics)
    : Property(identifier, displayName, invalidationLevel, semantics), PropertyOwner()
{}

CompositeProperty::~CompositeProperty() {}


void CompositeProperty::addProperty(Property* property) {
    subProperties_.push_back(property);
    property->addObserver(this);
}

void CompositeProperty::addProperty(Property& property) {
    addProperty(&property);
}

void CompositeProperty::setOwner(PropertyOwner* owner) {
    Property::setOwner(owner);

    for (size_t i=0; i<subProperties_.size(); i++)
        subProperties_[i]->setOwner(this);
}

void CompositeProperty::notify() {
    updateVisibility();
}

void CompositeProperty::updateVisibility() {
    for (size_t i=0; i<subProperties_.size(); i++)
        subProperties_[i]->updateVisibility();
}

void CompositeProperty::setVisible(bool val) {
    for (size_t i=0; i<subProperties_.size(); i++)
        subProperties_[i]->setVisible(val);
}

void CompositeProperty::setReadOnly(const bool& value) {
    for (size_t i=0; i<subProperties_.size(); i++)
        subProperties_[i]->setReadOnly(value);
}

void CompositeProperty::setPropertyModified(bool modified) {
    for (size_t i=0; i<subProperties_.size(); i++)
        subProperties_[i]->setPropertyModified(modified);

    Property::setPropertyModified(modified);
}

bool CompositeProperty::isPropertyModified() const {
    for (size_t i=0; i<subProperties_.size(); i++)
        if (subProperties_[i]->isPropertyModified()) return true;

    return false;
}

void CompositeProperty::set(const Property* srcProperty) {
    const CompositeProperty* compositeSrcProp = dynamic_cast<const CompositeProperty*>(srcProperty);

    if (compositeSrcProp) {
        std::vector<Property*> subProperties = compositeSrcProp->getSubProperties();

        if (subProperties.size()!=this->subProperties_.size()) {
            LogWarn("CompositeProperty mismatch. Unable to link");
            return;
        }

        for (size_t i=0; i<subProperties.size(); i++)
            this->subProperties_[i]->set(subProperties[i]);
    }
    else
        this->setVariant(const_cast<Property*>(srcProperty)->getVariant());

    propertyModified();
}


} // namespace
