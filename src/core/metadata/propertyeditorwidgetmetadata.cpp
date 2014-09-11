/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/core/metadata/propertyeditorwidgetmetadata.h>

namespace inviwo {

PropertyEditorWidgetDockStatus::PropertyEditorWidgetDockStatus() : dockStatus_("Floating") {}
PropertyEditorWidgetDockStatus::PropertyEditorWidgetDockStatus(std::string dockStatus)
    : dockStatus_(dockStatus) {}
PropertyEditorWidgetDockStatus::PropertyEditorWidgetDockStatus(
    const PropertyEditorWidgetDockStatus& rhs)
    : dockStatus_(rhs.dockStatus_) {};
PropertyEditorWidgetDockStatus& PropertyEditorWidgetDockStatus::operator=(
    const PropertyEditorWidgetDockStatus& that) {
    if (this != &that) dockStatus_ = that.dockStatus_;

    return *this;
}
bool PropertyEditorWidgetDockStatus::operator==(const PropertyEditorWidgetDockStatus& that) {
    if (this->getString() ==  that.getString())
        return true;

    return false;
}
const std::string& PropertyEditorWidgetDockStatus::getString() const {
    return dockStatus_;
}
const PropertyEditorWidgetDockStatus PropertyEditorWidgetDockStatus::Floating("Floating");
const PropertyEditorWidgetDockStatus PropertyEditorWidgetDockStatus::DockedLeft("DockedLeft");
const PropertyEditorWidgetDockStatus PropertyEditorWidgetDockStatus::DockedRight("DockedRight");


//////////////////////////////////////////////////////////////////////////

PropertyEditorWidgetMetaData::PropertyEditorWidgetMetaData()
    : positionMetaData_(ivec2(0))
    , dimensionMetaData_(ivec2(256))
    , visiblityMetaData_(false)
    , dockStatusMetaData_(PropertyEditorWidgetDockStatus::Floating.getString()) {}

PropertyEditorWidgetMetaData::PropertyEditorWidgetMetaData(const PropertyEditorWidgetMetaData& rhs)
    : positionMetaData_(rhs.positionMetaData_)
    , dimensionMetaData_(rhs.dimensionMetaData_)
    , visiblityMetaData_(rhs.visiblityMetaData_)
    , dockStatusMetaData_(PropertyEditorWidgetDockStatus::Floating.getString()) {}

PropertyEditorWidgetMetaData& PropertyEditorWidgetMetaData::operator=(
    const PropertyEditorWidgetMetaData& that) {
    if (this != &that) {
        positionMetaData_ = that.positionMetaData_;
        dimensionMetaData_ = that.dimensionMetaData_;
        visiblityMetaData_ = that.visiblityMetaData_;
        dockStatusMetaData_ = that.dockStatusMetaData_;
    }

    return *this;
}

PropertyEditorWidgetMetaData::~PropertyEditorWidgetMetaData() {}

PropertyEditorWidgetMetaData* PropertyEditorWidgetMetaData::clone() const {
    return new PropertyEditorWidgetMetaData(*this);
}

void PropertyEditorWidgetMetaData::setWidgetPosition(ivec2 pos) {
    positionMetaData_.set(pos);
}

ivec2 PropertyEditorWidgetMetaData::getWidgetPosition() {
    return positionMetaData_.get();
}

void PropertyEditorWidgetMetaData::setDimension(ivec2 dim) {
    dimensionMetaData_.set(dim);
}

ivec2 PropertyEditorWidgetMetaData::getDimension() const {
    return dimensionMetaData_.get();
}

void PropertyEditorWidgetMetaData::setVisibile(bool visibility) {
    visiblityMetaData_.set(visibility);
}

bool PropertyEditorWidgetMetaData::isVisible() const {
    return visiblityMetaData_.get();
}

void PropertyEditorWidgetMetaData::setDockStatus(PropertyEditorWidgetDockStatus& dockStatus) {
    dockStatusMetaData_.set(dockStatus.getString());
}

const PropertyEditorWidgetDockStatus PropertyEditorWidgetMetaData::getDocStatus() const {
    return PropertyEditorWidgetDockStatus(dockStatusMetaData_.get());
}

void PropertyEditorWidgetMetaData::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassIdentifier(), true);
    s.serialize("position", positionMetaData_.get());
    s.serialize("dimension", dimensionMetaData_.get());
    s.serialize("visibility", visiblityMetaData_.get());
    s.serialize("dockstatus", dockStatusMetaData_.get());
}

void PropertyEditorWidgetMetaData::deserialize(IvwDeserializer& d) {
    std::string className;
    ivec2 position, dimension;
    bool visibility;
    std::string dockStatus("");
    d.deserialize("type", className, true);
    d.deserialize("position", position);
    d.deserialize("dimension", dimension);
    d.deserialize("visibility", visibility);
    d.deserialize("dockstatus", dockStatus);

    if (dockStatus.empty())
        dockStatus="Floating";

    positionMetaData_.set(position);
    dimensionMetaData_.set(dimension);
    visiblityMetaData_.set(visibility);
    dockStatusMetaData_.set(dockStatus);
}

bool PropertyEditorWidgetMetaData::equal(const MetaData& rhs) const {
    const PropertyEditorWidgetMetaData* tmp =
        dynamic_cast<const PropertyEditorWidgetMetaData*>(&rhs);
    if (tmp) {
        return tmp->positionMetaData_ == positionMetaData_ &&
               tmp->visiblityMetaData_ == visiblityMetaData_ &&
               tmp->visiblityMetaData_ == visiblityMetaData_ &&
               tmp->dockStatusMetaData_ == dockStatusMetaData_;
    } else {
        return false;
    }
}

} // namespace
