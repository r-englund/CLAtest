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
#include <inviwo/core/util/variant.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/settings/systemsettings.h>

namespace inviwo {

std::map<std::string,std::string> Property::groupDisplayNames_;

Property::Property(std::string identifier,
                   std::string displayName,
                   PropertyOwner::InvalidationLevel invalidationLevel,
                   PropertySemantics semantics)
    : IvwSerializable()
    , identifier_(identifier)
    , displayName_(displayName)
    , readOnly_(false)
    , defaultReadOnly_(false)
    , semantics_(semantics)
    , visibilityMode_(APPLICATION)
    , propertyModified_(false)
    , invalidationLevel_(invalidationLevel)
    , owner_(NULL)
    , groupID_("")
    , groupDisplayName_("")
    , initiatingWidget_(NULL) {
}

Property::Property()
    : IvwSerializable()
    , identifier_("")
    , displayName_("")
    , readOnly_(false)
    , defaultReadOnly_(false)
    , semantics_(PropertySemantics::Default)
    , visibilityMode_(APPLICATION)
    , propertyModified_(false)
    , invalidationLevel_(PropertyOwner::INVALID_OUTPUT)
    , owner_(NULL)
    , groupID_("")
    , groupDisplayName_("")
    , initiatingWidget_(NULL)  {
}

Property::~Property() {
    while (!metaData_.empty()) {
        delete metaData_.back();
        metaData_.pop_back();
    }
}

std::string Property::getIdentifier() const {
    return identifier_;
}

void Property::setIdentifier(const std::string& identifier) {
    identifier_ = identifier;
}

std::string Property::getDisplayName() const {
    return displayName_;
}

void Property::setDisplayName(const std::string& displayName) {
    displayName_ = displayName;
}

PropertySemantics Property::getSemantics()const {
    return semantics_;
}

void Property::setSemantics(const PropertySemantics& semantics) {
    semantics_ = semantics;
}


void Property::setReadOnly(const bool& value) {
    readOnly_ = value;
    for(size_t i = 0; i < propertyWidgets_.size(); i++) {
        if(propertyWidgets_[i] != 0 && propertyWidgets_[i] != initiatingWidget_) {
            propertyWidgets_[i]->updateFromProperty();
        }
    }
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

void Property::setOwner(PropertyOwner* owner) {
    owner_ = owner;
}

void Property::registerWidget(PropertyWidget* propertyWidget) {
    propertyWidgets_.push_back(propertyWidget);

    if (this->visibilityMode_ == INVISIBLE)
        updateVisibility();
}

void Property::deregisterWidget(PropertyWidget* propertyWidget) {
    if (std::find(propertyWidgets_.begin(), propertyWidgets_.end(), propertyWidget)!=propertyWidgets_.end()) {
        propertyWidgets_.erase(std::remove(propertyWidgets_.begin(), propertyWidgets_.end(),
                                           propertyWidget), propertyWidgets_.end());
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

void Property::setGroupID(const std::string& groupID) {
    groupID_ = groupID;
}

std::string Property::getGroupID()const {
    return groupID_;
}

void Property::propertyModified() {
    onChangeCallback_.invokeAll();
    setPropertyModified(true);
    if (getOwner() && getInvalidationLevel() > PropertyOwner::VALID) {
        getOwner()->invalidate(getInvalidationLevel(), this);
    }
    updateWidgets();
}


void Property::setPropertyModified(bool modified) {
    propertyModified_ = modified;
}

bool Property::isPropertyModified() const {
    return propertyModified_;
}

Variant Property::getVariant() {
    return Variant(getVariantType());
}

void Property::setVariant(const Variant&) {}

int Property::getVariantType() {
    return Variant::VariantTypeInvalid;
}

MetaData* Property::getMetaData(std::string className) {
    //TODO: Code is very similar to processor getMetaData. Use common scheme
    MetaData* meta = 0;

    for (size_t i=0; i<metaData_.size(); i++) {
        if (metaData_[i]->getClassName()==className) {
            meta = metaData_[i];
            break;
        }
    }

    if (!meta) {
        meta = dynamic_cast<MetaData*>(MetaDataFactory::getRef().create(className));
        metaData_.push_back(meta);
    }

    return meta;
}

void Property::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassName(), true);
    s.serialize("identifier", identifier_, true);
    s.serialize("displayName", displayName_, true);
    s.serialize("semantics", semantics_);
    s.serialize("visibilityMode", visibilityMode_);
    if (readOnly_ != defaultReadOnly_) {
        s.serialize("readonly", readOnly_);
    }
    if (metaData_.size() > 0) {
        s.serialize("MetaDataList", metaData_, "MetaData");
    }
}

void Property::deserialize(IvwDeserializer& d) {
    std::string className;
    d.deserialize("type", className, true);
    d.deserialize("identifier", identifier_, true);
    d.deserialize("displayName", displayName_, true);
    d.deserialize("semantics", semantics_);
    int mode = visibilityMode_;
    d.deserialize("visibilityMode", mode);
    visibilityMode_ = static_cast<PropertyVisibilityMode>(mode);
    d.deserialize("readonly", readOnly_);
    d.deserialize("MetaDataList", metaData_, "MetaData");
    updateVisibility();
}

bool Property::operator==(const Property& prop) {
    // TODO: this is not an equal operator, change to isSameType or similar
    if (this->getClassName()==prop.getClassName()) return true;
    else return false;
}

void Property::setGroupDisplayName(const std::string& groupID, const std::string& groupDisplayName) {
    Property::groupDisplayNames_.insert(std::pair<std::string,std::string>(groupID, groupDisplayName));
}

std::string Property::getGroupDisplayName() const {
    return groupDisplayNames_[groupID_];
}

void Property::setVisibility(PropertyVisibilityMode visibilityMode) {
    this->visibilityMode_ = visibilityMode;
    updateVisibility();
}

void Property::updateVisibility() {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    Settings* mainSettings = inviwoApp->getSettingsByType<SystemSettings>();
    PropertyVisibilityMode appMode = static_cast<PropertyVisibilityMode>(
        dynamic_cast<OptionPropertyInt*>(mainSettings->getPropertyByIdentifier("viewMode"))->get());

    if (visibilityMode_ == INVISIBLE) {
        for (size_t i = 0; i < propertyWidgets_.size(); i++) propertyWidgets_[i]->hideWidget();
    }

    if (visibilityMode_ == APPLICATION) {
        for (size_t i = 0; i < propertyWidgets_.size(); i++) propertyWidgets_[i]->showWidget();
    } else if (visibilityMode_ == DEVELOPMENT && appMode == DEVELOPMENT) {
        for (size_t i = 0; i < propertyWidgets_.size(); i++) propertyWidgets_[i]->showWidget();
    } else if (visibilityMode_ == DEVELOPMENT && appMode == APPLICATION) {
        for (size_t i = 0; i < propertyWidgets_.size(); i++) propertyWidgets_[i]->hideWidget();
    }
}

void Property::setVisible(bool val) {
    if (val)
        setVisibility(APPLICATION);
    else
        setVisibility(INVISIBLE);
}

void Property::setCurrentStateAsDefault() {
    defaultReadOnly_ = readOnly_;
}

void Property::resetToDefaultState() {
    readOnly_ = defaultReadOnly_;
    propertyModified();
}

void Property::set(const Property* src) {
    readOnly_ = src->readOnly_;
    defaultReadOnly_ = src->defaultReadOnly_;
    propertyModified();
}

} // namespace
