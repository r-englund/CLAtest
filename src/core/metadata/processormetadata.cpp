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
 * Main file authors: Sathish Kottravel, Timo Ropinski
 *
 *********************************************************************************/

#include <inviwo/core/metadata/processormetadata.h>

namespace inviwo {

ProcessorMetaData::ProcessorMetaData() {
    visiblityMetaData_.set(true);
    selectionMetaData_.set(false);
}

ProcessorMetaData::ProcessorMetaData(const ProcessorMetaData& rhs)
    : positionMetaData_(rhs.positionMetaData_)
    , visiblityMetaData_(rhs.visiblityMetaData_)
    , selectionMetaData_(rhs.selectionMetaData_) {
}

ProcessorMetaData& ProcessorMetaData::operator=(const ProcessorMetaData& that) {
    if (this != &that) {
        positionMetaData_ = that.positionMetaData_;
        visiblityMetaData_ = that.visiblityMetaData_;
        selectionMetaData_ = that.selectionMetaData_;
    }

    return *this;
}

ProcessorMetaData::~ProcessorMetaData() {}

ProcessorMetaData* ProcessorMetaData::clone() const {
    return new ProcessorMetaData(*this);
}

void ProcessorMetaData::setPosition(ivec2 pos) {
    positionMetaData_.set(pos);
}

ivec2 ProcessorMetaData::getPosition() {
    return positionMetaData_.get();
}

void ProcessorMetaData::setVisibile(bool visibility) {
    visiblityMetaData_.set(visibility);
}

bool ProcessorMetaData::isVisible() {
    return visiblityMetaData_.get();
}

void ProcessorMetaData::setSelected(bool selection) {
    selectionMetaData_.set(selection);
}

bool ProcessorMetaData::isSelected() {
    return selectionMetaData_.get();
}

void ProcessorMetaData::serialize(IvwSerializer& s) const {
    //   CompositeMetaData::serialize(s);
    s.serialize("type", getClassName(), true);
    s.serialize("position", positionMetaData_.get());
    s.serialize("visibility", visiblityMetaData_.get());
    s.serialize("selection", selectionMetaData_.get());
}

void ProcessorMetaData::deserialize(IvwDeserializer& d) {
    std::string className;
    ivec2 position(positionMetaData_.get());
    bool visibility=visiblityMetaData_.get();
    bool selection=selectionMetaData_.get();
    //   CompositeMetaData::deserialize(d);
    d.deserialize("type", className, true);
    d.deserialize("position", position);
    d.deserialize("visibility", visibility);
    d.deserialize("selection", selection);
    //TODO check if position is within current monitors
    positionMetaData_.set(position);
    visiblityMetaData_.set(visibility);
    selectionMetaData_.set(selection);
}

bool ProcessorMetaData::equal(const MetaData& rhs) const {
    const ProcessorMetaData* tmp = dynamic_cast<const ProcessorMetaData*>(&rhs);
    if (tmp) {
        return operator==<ivec2>(tmp->positionMetaData_, positionMetaData_)
            && operator==<bool>(tmp->visiblityMetaData_, visiblityMetaData_)
            && operator==<bool>(tmp->selectionMetaData_, selectionMetaData_);
    } else {
        return false;
    }
}



} // namespace
