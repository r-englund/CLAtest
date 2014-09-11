/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file authors: Sathish Kottravel, Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/core/metadata/processorwidgetmetadata.h>

namespace inviwo {

ProcessorWidgetMetaData::ProcessorWidgetMetaData()
    : MetaData()
    , positionMetaData_(ivec2(0))
    , dimensionMetaData_(ivec2(256))
    , visiblityMetaData_(false) {
}

ProcessorWidgetMetaData::ProcessorWidgetMetaData(const ProcessorWidgetMetaData& rhs)
    : MetaData()
    , positionMetaData_(rhs.positionMetaData_)
    , dimensionMetaData_(rhs.dimensionMetaData_)
    , visiblityMetaData_(rhs.visiblityMetaData_) {
}

ProcessorWidgetMetaData& ProcessorWidgetMetaData::operator=(const ProcessorWidgetMetaData& that) {
    if (this != &that) {
        positionMetaData_ = that.positionMetaData_;
        dimensionMetaData_ = that.dimensionMetaData_;
        visiblityMetaData_ = that.visiblityMetaData_;
    }

    return *this;
}

ProcessorWidgetMetaData::~ProcessorWidgetMetaData() {}

ProcessorWidgetMetaData* ProcessorWidgetMetaData::clone() const {
    return new ProcessorWidgetMetaData(*this);
}

void ProcessorWidgetMetaData::setWidgetPosition(ivec2 pos) {
    positionMetaData_.set(pos);
}

ivec2 ProcessorWidgetMetaData::getWidgetPosition() {
    return positionMetaData_.get();
}

void ProcessorWidgetMetaData::setDimension(ivec2 dim) {
    dimensionMetaData_.set(dim);
}

ivec2 ProcessorWidgetMetaData::getDimension() const {
    return dimensionMetaData_.get();
}

void ProcessorWidgetMetaData::setVisibile(bool visibility) {
    visiblityMetaData_.set(visibility);
}

bool ProcessorWidgetMetaData::isVisible() const {
    return visiblityMetaData_.get();
}

void ProcessorWidgetMetaData::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassIdentifier(), true);
    s.serialize("position", positionMetaData_.get());
    s.serialize("dimension", dimensionMetaData_.get());
    s.serialize("visibility", visiblityMetaData_.get());
}

void ProcessorWidgetMetaData::deserialize(IvwDeserializer& d) {
    std::string className;
    ivec2 position, dimension;
    bool visibility;
    d.deserialize("type", className, true);
    d.deserialize("position", position);
    d.deserialize("dimension", dimension);
    d.deserialize("visibility", visibility);
    positionMetaData_.set(position);
    dimensionMetaData_.set(dimension);
    visiblityMetaData_.set(visibility);
}

bool ProcessorWidgetMetaData::equal(const MetaData& rhs) const {
    const ProcessorWidgetMetaData* tmp = dynamic_cast<const ProcessorWidgetMetaData*>(&rhs);
    if (tmp) {
        return tmp->positionMetaData_ == positionMetaData_
            && tmp->visiblityMetaData_ == visiblityMetaData_
            && tmp->dimensionMetaData_ == dimensionMetaData_;
    } else {
        return false;
    }
}





} // namespace
