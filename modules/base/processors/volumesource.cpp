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
    , lengths_("length_", "Lengths", vec3(1.0f), vec3(0.0f), vec3(10.0f))
    , angels_("angles_", "Angles", vec3(90.0f), vec3(0.0f), vec3(180.0f), vec3(1.0f))
    , offset_("offset_", "Offset", vec3(0.0f), vec3(-10.0f), vec3(10.0f)) {

    DataSource<Volume, VolumeOutport>::file_.setDisplayName("Volume file");
    dataRange_.onChange(this, &VolumeSource::invalidateOutput);
    addProperty(dataRange_);
    valueRange_.onChange(this, &VolumeSource::invalidateOutput);
    addProperty(valueRange_);
    valueUnit_.onChange(this, &VolumeSource::invalidateOutput);
    addProperty(valueUnit_);

    overRideDefaults_.setGroupDisplayName("Basis", "Basis and offset");
    overRideDefaults_.setGroupID("Basis");
    lengths_.setGroupID("Basis");
    angels_.setGroupID("Basis");
    offset_.setGroupID("Basis");
    lengths_.setReadOnly(true);
    angels_.setReadOnly(true);
    offset_.setReadOnly(true);

    orgLengths_ = lengths_.get();
    orgAngles_ = angels_.get();
    orgOffet_ = offset_.get();

    overRideDefaults_.onChange(this, &VolumeSource::onOverrideChange);

    addProperty(overRideDefaults_);
    addProperty(lengths_);
    addProperty(angels_);
    addProperty(offset_);
}

VolumeSource::~VolumeSource() {}

void VolumeSource::onOverrideChange() {
    if(!overRideDefaults_.get()) {
        vec3 tmpLength = lengths_.get();
        vec3 tmpAngle = angels_.get();
        vec3 tmpOffset = offset_.get();       
        lengths_.set(orgLengths_);
        angels_.set(orgAngles_);
        offset_.set(orgOffet_);
        orgLengths_ = tmpLength;
        orgAngles_ = tmpAngle;
        orgOffet_ = tmpOffset;
        lengths_.setReadOnly(true);
        angels_.setReadOnly(true);
        offset_.setReadOnly(true);

    } else {
        vec3 tmpLength = lengths_.get();
        vec3 tmpAngle = angels_.get();
        vec3 tmpOffset = offset_.get();
        lengths_.set(orgLengths_);
        angels_.set(orgAngles_);
        offset_.set(orgOffet_);
        orgLengths_ = tmpLength;
        orgAngles_ = tmpAngle;
        orgOffet_ = tmpOffset;
        lengths_.setReadOnly(false);
        angels_.setReadOnly(false);
        offset_.setReadOnly(false);
    }
}

void VolumeSource::invalidateOutput() {
//    Volume* volume = DataSource<Volume, VolumeOutport>::port_.getData();
//
//    if (volume) {
//        volume->setMetaData<Vec2MetaData>("DataRange", dataRange_.get());
//        volume->setMetaData<Vec2MetaData>("ValueRange", valueRange_.get());
//        volume->setMetaData<StringMetaData>("ValueUnit", valueUnit_.get());
//    }
//
//    DataSource<Volume, VolumeOutport>::invalidateOutput();
}

void VolumeSource::dataLoaded(Volume* volume) {
    float max = static_cast<float>(volume->getDataFormat()->getMax());
    float min = static_cast<float>(volume->getDataFormat()->getMin());
    //dataRange_.setRangeMin(min);
    //dataRange_.setRangeMax(max);

    //if (volume->hasMetaData<DVec2MetaData>("DataRange")) {
    //    dataRange_.set(volume->getMetaData<Vec2MetaData>("DataRange", dataRange_.get()));
    //} else {
    //    dataRange_.set(vec2(min, max));
    //}

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
    angels_.set(vec3(alpha, beta, gamma));
    offset_.set(offset);

    lengths_.setCurrentStateAsDefault();
    angels_.setCurrentStateAsDefault();
    offset_.setCurrentStateAsDefault();

    orgLengths_ = lengths_.get();
    orgAngles_ = angels_.get();
    orgOffet_ = offset_.get();

    //valueRange_.set(volume->getMetaData<Vec2MetaData>("ValueRange", dataRange_.get()));
    //valueUnit_.set(volume->getMetaData<StringMetaData>("ValueUnit", valueUnit_.get()));
    //invalidateOutput();
}

void VolumeSource::process() {
    Volume* out;

    if(port_.hasData()) {
        out = port_.getData();

        float a = lengths_.get()[0];
        float b = lengths_.get()[1];
        float c = lengths_.get()[2];
        vec3 offset = offset_.get();
        float alpha = glm::radians(angels_.get()[0]);
        float beta = glm::radians(angels_.get()[1]);
        float gamma = glm::radians(angels_.get()[2]);
        float v = std::sqrt(1 - std::cos(alpha)*std::cos(alpha) - std::cos(beta)*std::cos(beta) - std::cos(gamma)*std::cos(gamma)
                            - 2 * std::cos(alpha)*std::cos(beta)*std::cos(gamma));
        mat4 newBasisAndOffset(
            a, b*std::cos(gamma), c*std::cos(beta), offset[0],
            0.0f, b*std::sin(gamma), c*(std::cos(alpha) - std::cos(beta)*std::cos(gamma)) / std::sin(gamma), offset[1],
            0.0f, 0.0f, c*v / std::sin(gamma), offset[2],
            0.0f, 0.0f, 0.0f, 1.0f
            );
        out->setBasisAndOffset(glm::transpose(newBasisAndOffset));
    }
}




} // namespace
