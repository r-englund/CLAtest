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
 * Main file authors: Sathish Kottravel, Rickard Englund
 *
 *********************************************************************************/

#include <inviwo/core/properties/property.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/settings/systemsettings.h>

namespace inviwo {

PropertyClassIdentifier(Property, "org.inviwo.undefined");

Property::Property(const std::string &identifier,
                   const std::string &displayName,
                   PropertyOwner::InvalidationLevel invalidationLevel,
                   PropertySemantics semantics)
    : IvwSerializable()
    , identifier_(identifier)
    , displayName_(displayName)
    , readOnly_(false)
    , defaultReadOnly_(false)
    , semantics_(semantics)
    , defaultSemantics_(semantics)
    , usageMode_(APPLICATION)
    , visible_(true)
    , propertyModified_(false)
    , invalidationLevel_(invalidationLevel)
    , owner_(NULL)
    , initiatingWidget_(NULL) {
}

Property::Property()
    : IvwSerializable()
    , identifier_("")
    , displayName_("")
    , readOnly_(false)
    , defaultReadOnly_(false)
    , semantics_(PropertySemantics::Default)
    , defaultSemantics_(PropertySemantics::Default)
    , usageMode_(APPLICATION)
    , visible_(true)
    , propertyModified_(false)
    , invalidationLevel_(PropertyOwner::INVALID_OUTPUT)
    , owner_(NULL)
    , initiatingWidget_(NULL)  {
}

Property::~Property() {
}

std::string Property::getIdentifier() const {
    return identifier_;
}
std::vector<std::string> Property::getPath() const {
    std::vector<std::string> path;
    if(owner_) {
        path = owner_->getPath();
    } 
    path.push_back(identifier_);
    return path;
}


void Property::setIdentifier(const std::string& identifier) {
    identifier_ = identifier;
}

std::string Property::getDisplayName() const {
    return displayName_;
}

void Property::setDisplayName(const std::string& displayName) {
    displayName_ = displayName;
    updateWidgets();
}

PropertySemantics Property::getSemantics()const {
    return semantics_;
}

void Property::setSemantics(const PropertySemantics& semantics) {
    semantics_ = semantics;
}


void Property::setReadOnly(const bool& value) {
    readOnly_ = value;
    updateWidgets();
}

bool Property::getReadOnly()const {
    return readOnly_;
}

PropertyOwner::InvalidationLevel Property::getInvalidationLevel() const {
    return invalidationLevel_;
}
void Property::setInvalidationLevel(PropertyOwner::InvalidationLevel invalidationLevel) {
    invalidationLevel_ = invalidationLevel;
}

PropertyOwner* Property::getOwner() {
    return owner_;
}


const PropertyOwner* Property::getOwner() const {
    return owner_;
}

void Property::setOwner(PropertyOwner* owner) {
    owner_ = owner;
}

void Property::registerWidget(PropertyWidget* propertyWidget) {
    propertyWidgets_.push_back(propertyWidget);
}

void Property::deregisterWidget(PropertyWidget* propertyWidget) {
    if (std::find(propertyWidgets_.begin(), propertyWidgets_.end(), propertyWidget) !=
        propertyWidgets_.end()) {
        propertyWidgets_.erase(
            std::remove(propertyWidgets_.begin(), propertyWidgets_.end(), propertyWidget),
            propertyWidgets_.end());
    }
}

void Property::setInitiatingWidget(PropertyWidget* propertyWidget){
    initiatingWidget_ = propertyWidget;
}
void Property::clearInitiatingWidget() {
    initiatingWidget_ = NULL;
}
void Property::updateWidgets() {
    for(size_t i = 0; i < propertyWidgets_.size(); i++) {
        if(propertyWidgets_[i] != 0 && propertyWidgets_[i] != initiatingWidget_) {
            propertyWidgets_[i]->updateFromProperty();
        }
    }
}

bool Property::hasWidgets() const {
    return !propertyWidgets_.empty();
}

void Property::propertyModified() {
    onChangeCallback_.invokeAll();
    setPropertyModified(true);

    PropertyOwner* owner = getOwner();
    if(owner) {

        // Evaluate property links
        Processor* processor = owner->getProcessor();
        if (processor) {
             processor->notifyObserversAboutPropertyChange(this);
        }

        // Invalidate Owner
        if (getInvalidationLevel() > PropertyOwner::VALID) {
            owner->invalidate(getInvalidationLevel(), this);        
        }
    }

    updateWidgets();
}


void Property::setPropertyModified(bool modified) {
    propertyModified_ = modified;
}

bool Property::isPropertyModified() const {
    return propertyModified_;
}

void Property::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassIdentifier(), true);
    s.serialize("identifier", identifier_, true);
    s.serialize("displayName", displayName_, true);
    if (semantics_ != defaultSemantics_) {
        s.serialize("semantics", semantics_);
    }
    s.serialize("usageMode", usageMode_);
    s.serialize("visible", visible_);
    if (readOnly_ != defaultReadOnly_) {
        s.serialize("readonly", readOnly_);
    }
    MetaDataOwner::serialize(s);
}

void Property::deserialize(IvwDeserializer& d) {
    std::string className;
    d.deserialize("type", className, true);
    d.deserialize("identifier", identifier_, true);
    d.deserialize("displayName", displayName_, true);
    d.deserialize("semantics", semantics_);
    int mode = usageMode_;
    d.deserialize("usageMode", mode);
    usageMode_ = static_cast<UsageMode>(mode);
    d.deserialize("visible", visible_);
    d.deserialize("readonly", readOnly_);
    updateVisibility();
    MetaDataOwner::deserialize(d);
}

void Property::setUsageMode(UsageMode visibilityMode) {
    this->usageMode_ = visibilityMode;
    updateVisibility();
}

void Property::updateVisibility() {
    UsageMode appMode =
        InviwoApplication::getPtr()->getSettingsByType<SystemSettings>()->getApplicationUsageMode();

    UsageMode mode = getUsageMode();

    if (getVisible() == false) {
        for (size_t i = 0; i < propertyWidgets_.size(); i++) propertyWidgets_[i]->hideWidget();
    }

    if (mode == APPLICATION) {
        for (size_t i = 0; i < propertyWidgets_.size(); i++) propertyWidgets_[i]->showWidget();
    } else if (mode == DEVELOPMENT && appMode == DEVELOPMENT) {
        for (size_t i = 0; i < propertyWidgets_.size(); i++) propertyWidgets_[i]->showWidget();
    } else if (mode == DEVELOPMENT && appMode == APPLICATION) {
        for (size_t i = 0; i < propertyWidgets_.size(); i++) propertyWidgets_[i]->hideWidget();
    }

}

void Property::setVisible(bool val) {
    visible_ = val;
    updateVisibility();
}
bool Property::getVisible() {
    return visible_;
}


void Property::setCurrentStateAsDefault() {
    defaultReadOnly_ = readOnly_;
    defaultSemantics_ = semantics_;
}

void Property::resetToDefaultState() {
    readOnly_ = defaultReadOnly_;
    propertyModified();
}

void Property::set(const Property* src) {
    propertyModified();
}

} // namespace
