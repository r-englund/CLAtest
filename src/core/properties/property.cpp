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
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

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
    : VoidObservable()
    , identifier_(identifier)
    , displayName_(displayName)
    , readOnly_(false)
    , semantics_(semantics)
    , visibilityMode_(APPLICATION)
    , propertyModified_(false)
    , invalidationLevel_(invalidationLevel)
    , owner_(0)
    , groupID_("") {
}

Property::Property()
    : identifier_("")
    , displayName_("")
    , propertyModified_(false) {
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

void Property::updateWidgets() {
    for (size_t i=0; i<propertyWidgets_.size(); i++)
        if (propertyWidgets_[i] != 0)
            propertyWidgets_[i]->updateFromProperty();
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
    onChangeCallback_.invoke();
    setPropertyModified(true);

    //FIXME: if set() is called before addProperty(), getOwner() will be 0 ( case for option properties )
    if (getOwner()) getOwner()->invalidate(getInvalidationLevel(), this);

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
    s.serialize("MetaDataList", metaData_, "MetaData") ;
}

void Property::deserialize(IvwDeserializer& d) {
    std::string className;
    d.deserialize("type", className, true);
    d.deserialize("identifier", identifier_, true);
    d.deserialize("displayName", displayName_, true);
    d.deserialize("MetaDataList", metaData_, "MetaData") ;
}

bool Property::operator==(const Property& prop) {
    // TODO: this is not an equal operator, change to isSameType or similar
    if (this->getClassName()==prop.getClassName()) return true;
    else return false;
}

void Property::setGroupDisplayName(const std::string& groupID,const std::string& groupDisplayName) {
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
    PropertyVisibilityMode appMode =  static_cast<PropertyVisibilityMode>(dynamic_cast<OptionPropertyInt*>
                                      (mainSettings->getPropertyByIdentifier("viewMode"))->get());

    if (visibilityMode_ == INVISIBLE) {
        for (size_t i=0; i<propertyWidgets_.size(); i++)
            propertyWidgets_[i]->hideWidget();
    }

    if (visibilityMode_ == APPLICATION) {
        for (size_t i=0; i<propertyWidgets_.size(); i++)
            propertyWidgets_[i]->showWidget();
    }
    else if (visibilityMode_ == DEVELOPMENT && appMode == DEVELOPMENT) {
        for (size_t i=0; i<propertyWidgets_.size(); i++)
            propertyWidgets_[i]->showWidget();
    }
    else if (visibilityMode_ == DEVELOPMENT && appMode == APPLICATION) {
        for (size_t i=0; i<propertyWidgets_.size(); i++)
            propertyWidgets_[i]->hideWidget();
    }
}

void Property::setVisible(bool val) {
    if (val)
        setVisibility(APPLICATION);
    else
        setVisibility(INVISIBLE);
}

} // namespace
