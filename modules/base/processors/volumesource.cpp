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
 * Main file authors: Timo Ropinski, Peter Steneteg
 *
 *********************************************************************************/

#include "volumesource.h"
#include <inviwo/core/resources/resourcemanager.h>
#include <inviwo/core/resources/templateresource.h>
#include <inviwo/core/util/urlparser.h>
#include <inviwo/core/io/datareaderfactory.h>
#include <inviwo/core/io/rawvolumereader.h>
#include <inviwo/core/network/processornetwork.h>
#include <glm/gtx/vector_angle.hpp>
#include <math.h>

namespace inviwo {

ProcessorClassName(VolumeSource, "VolumeSource");
ProcessorCategory(VolumeSource, "Data Input");
ProcessorCodeState(VolumeSource, CODE_STATE_EXPERIMENTAL);

VolumeSource::VolumeSource()
    : DataSource<Volume, VolumeOutport>()
    , dataRange_("dataRange", "Data range", 0.f, 255.0f, 0.f, 255.0f)
    , valueRange_("valueRange", "Value range", 0.f, 255.0f, 0.f, 255.0f)
    , valueUnit_("valueUnit", "Value unit", "")
    , overRideDefaults_("override", "Override", false)
    , lengths_("length", "Lengths", vec3(1.0f), vec3(0.0f), vec3(10.0f))
    , angles_("angles", "Angles", vec3(90.0f), vec3(0.0f), vec3(180.0f), vec3(1.0f))
    , offset_("offset", "Offset", vec3(0.0f), vec3(-10.0f), vec3(10.0f))
    , dimensions_("dimensions", "Dimensions")
    , format_("format", "Format","") {

    DataSource<Volume, VolumeOutport>::file_.setDisplayName("Volume file");
    
    overRideDefaults_.setGroupDisplayName("Basis", "Basis and offset");
    overRideDefaults_.setGroupID("Basis");
    lengths_.setGroupID("Basis");
    angles_.setGroupID("Basis");
    offset_.setGroupID("Basis");
    lengths_.setReadOnly(true);
    angles_.setReadOnly(true);
    offset_.setReadOnly(true);
    dimensions_.setReadOnly(true);
    format_.setReadOnly(true);
    dimensions_.setCurrentStateAsDefault();
    format_.setCurrentStateAsDefault();

    dimensions_.setGroupDisplayName("Information", "Data information");
    dimensions_.setGroupID("Information");
    format_.setGroupID("Information");
    dataRange_.setGroupID("Information");
    valueRange_.setGroupID("Information");
    valueUnit_.setGroupID("Information");

    orgLengths_ = lengths_.get();
    orgAngles_ = angles_.get();
    orgOffet_ = offset_.get();

    overRideDefaults_.onChange(this, &VolumeSource::onOverrideChange);

    addProperty(dimensions_);
    addProperty(format_);

    addProperty(dataRange_);
    addProperty(valueRange_);
    addProperty(valueUnit_);

    addProperty(overRideDefaults_);
    addProperty(lengths_);
    addProperty(angles_);
    addProperty(offset_);
}

VolumeSource::~VolumeSource() {}

void VolumeSource::onOverrideChange() {
    if (this->isDeserializing_) {
        return;
    }
    
    if(!overRideDefaults_.get()) {
        vec3 tmpLength = lengths_.get();
        vec3 tmpAngle = angles_.get();
        vec3 tmpOffset = offset_.get();       
        lengths_.set(orgLengths_);
        angles_.set(orgAngles_);
        offset_.set(orgOffet_);
        orgLengths_ = tmpLength;
        orgAngles_ = tmpAngle;
        orgOffet_ = tmpOffset;
        lengths_.setReadOnly(true);
        angles_.setReadOnly(true);
        offset_.setReadOnly(true);

    } else {
        vec3 tmpLength = lengths_.get();
        vec3 tmpAngle = angles_.get();
        vec3 tmpOffset = offset_.get();
        lengths_.set(orgLengths_);
        angles_.set(orgAngles_);
        offset_.set(orgOffet_);
        orgLengths_ = tmpLength;
        orgAngles_ = tmpAngle;
        orgOffet_ = tmpOffset;
        lengths_.setReadOnly(false);
        angles_.setReadOnly(false);
        offset_.setReadOnly(false);
    }
}

void VolumeSource::dataLoaded(Volume* volume) {
    InviwoApplication::getPtr()->getProcessorNetwork()->lock();
    // We should use double here... but at the moment there is no double min max widget...
    float max = static_cast<float>(volume->getDataFormat()->getMax());
    float min = static_cast<float>(volume->getDataFormat()->getMin());
    
    // Default values
    dataRange_.setRangeMin(min);
    dataRange_.setRangeMax(max);
    valueRange_.setRangeMin(min);
    valueRange_.setRangeMax(max);
    dataRange_.set(vec2(min, max));
    valueRange_.set(vec2(min, max));
    valueUnit_.set("No unit");

    // Overide with metadata from volume
    dataRange_.set(volume->getMetaData<Vec2MetaData>("DataRange", dataRange_.get()));
    valueRange_.set(volume->getMetaData<Vec2MetaData>("ValueRange", valueRange_.get()));
    valueUnit_.set(volume->getMetaData<StringMetaData>("ValueUnit", valueUnit_.get()));

    // calculate and set properties.
    vec3 a(volume->getBasis()[0]);
    vec3 b(volume->getBasis()[1]);
    vec3 c(volume->getBasis()[2]);
    vec3 offset(volume->getOffset());

    float alpha = glm::degrees(glm::angle(b, c));
    float beta = glm::degrees(glm::angle(c, a));
    float gamma = glm::degrees(glm::angle(a, b));

    lengths_.setMaxValue(vec3(2.0f * (glm::length(a) + glm::length(b) + glm::length(c))));
    offset_.setMaxValue(vec3(5.0*glm::length(offset)));
    offset_.setMinValue(vec3(-5.0*glm::length(offset)));

    lengths_.set(vec3(glm::length(a), glm::length(b), glm::length(c)));
    angles_.set(vec3(alpha, beta, gamma));
    offset_.set(offset);

    lengths_.setCurrentStateAsDefault();
    angles_.setCurrentStateAsDefault();
    offset_.setCurrentStateAsDefault();

    orgLengths_ = lengths_.get();
    orgAngles_ = angles_.get();
    orgOffet_ = offset_.get();

    dimensions_.set(glm::to_string(volume->getDimension()));
    format_.set(volume->getDataFormat()->getString());
    
    InviwoApplication::getPtr()->getProcessorNetwork()->unlock();
    invalidateOutput();
}

void VolumeSource::process() {
    Volume* out;

    if (this->isDeserializing_) {
        return;
    }

    if(port_.hasData()) {
        out = port_.getData();

        float a = lengths_.get()[0];
        float b = lengths_.get()[1];
        float c = lengths_.get()[2];
        vec3 offset = offset_.get();
        float alpha = glm::radians(angles_.get()[0]);
        float beta = glm::radians(angles_.get()[1]);
        float gamma = glm::radians(angles_.get()[2]);
        float v = std::sqrt(1 - std::cos(alpha)*std::cos(alpha) - std::cos(beta)*std::cos(beta) - std::cos(gamma)*std::cos(gamma)
                            - 2 * std::cos(alpha)*std::cos(beta)*std::cos(gamma));
        mat4 newBasisAndOffset(
            a, b*std::cos(gamma), c*std::cos(beta), offset[0],
            0.0f, b*std::sin(gamma), c*(std::cos(alpha) - std::cos(beta)*std::cos(gamma)) / std::sin(gamma), offset[1],
            0.0f, 0.0f, c*v / std::sin(gamma), offset[2],
            0.0f, 0.0f, 0.0f, 1.0f
            );
        out->setBasisAndOffset(glm::transpose(newBasisAndOffset));

        out->setMetaData<Vec2MetaData>("DataRange", dataRange_.get());
        out->setMetaData<Vec2MetaData>("ValueRange", valueRange_.get());
        out->setMetaData<StringMetaData>("ValueUnit", valueUnit_.get());
    }
}

void VolumeSource::serialize(IvwSerializer& s) const {
    DataSource<Volume, VolumeOutport>::serialize(s);
}

void VolumeSource::deserialize(IvwDeserializer& d) {
    DataSource<Volume, VolumeOutport>::deserialize(d);
    onOverrideChange();
}




} // namespace
