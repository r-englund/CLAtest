 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file authors: Sathish Kottravel, Timo Ropinski, Alexander Johansson
 *
 *********************************************************************************/

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
