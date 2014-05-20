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
 * Main file authors: Peter Steneteg
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
    , dataRange_("dataRange", "Data range", 0., 255.0, -DataFLOAT64::max(), DataFLOAT64::max(), 0.0,
                 0.0, PropertyOwner::INVALID_OUTPUT, PropertySemantics("Text"))
    , valueRange_("valueRange", "Value range", 0., 255.0, -DataFLOAT64::max(), DataFLOAT64::max(),
                  0.0, 0.0, PropertyOwner::INVALID_OUTPUT, PropertySemantics("Text"))
    , valueUnit_("valueUnit", "Value unit", "arb. unit.")
    , overRideDefaults_("override", "Override", false)
    , lengths_("length", "Lengths", vec3(1.0f), vec3(0.0f), vec3(10.0f))
    , angles_("angles", "Angles", vec3(90.0f), vec3(0.0f), vec3(180.0f), vec3(1.0f))
    , offset_("offset", "Offset", vec3(0.0f), vec3(-10.0f), vec3(10.0f))
    , dimensions_("dimensions", "Dimensions")
    , format_("format", "Format", "")
    , isDeserializing_(false) {

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

    overrideLengths_ = lengths_.get();
    overrideAngles_ = angles_.get();
    overrideOffset_ = offset_.get();

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

    if (!overRideDefaults_.get()) {
        overrideLengths_ = lengths_.get();
        overrideAngles_ = angles_.get();
        overrideOffset_ = offset_.get();
        lengths_.resetToDefaultState();
        angles_.resetToDefaultState();
        offset_.resetToDefaultState();
        lengths_.setReadOnly(true);
        angles_.setReadOnly(true);
        offset_.setReadOnly(true);
    } else {
        lengths_.set(overrideLengths_);
        angles_.set(overrideAngles_);
        offset_.set(overrideOffset_);
        lengths_.setReadOnly(false);
        angles_.setReadOnly(false);
        offset_.setReadOnly(false);
    }
}

void VolumeSource::dataLoaded(Volume* volume) {
    InviwoApplication::getPtr()->getProcessorNetwork()->lock();
    
    // Save the old state, used when we have deserialized to be able to restore deserilized values
    // after the file loading have read in the default values.
    saveState();

    // Set the data range from the volume
    dataRange_.set(volume->dataMap_.dataRange);
    valueRange_.set(volume->dataMap_.valueRange);
    valueUnit_.set(volume->dataMap_.valueUnit);
    
    // calculate and set properties basis properties.
    vec3 a(volume->getBasis()[0]);
    vec3 b(volume->getBasis()[1]);
    vec3 c(volume->getBasis()[2]);
    vec3 offset(volume->getOffset());
    float alpha = glm::degrees(glm::angle(b, c));
    float beta = glm::degrees(glm::angle(c, a));
    float gamma = glm::degrees(glm::angle(a, b));
    lengths_.setMaxValue(vec3(2.0f * (glm::length(a) + glm::length(b) + glm::length(c))));
    offset_.setMaxValue(vec3(5.0 * glm::length(offset)));
    offset_.setMinValue(vec3(-5.0 * glm::length(offset)));
    lengths_.set(vec3(glm::length(a), glm::length(b), glm::length(c)));
    angles_.set(vec3(alpha, beta, gamma));
    offset_.set(offset);

    // Display the format and dimension, read only.
    std::stringstream ss;
    ss << volume->getDimension().x << " x "
       << volume->getDimension().y << " x "
       << volume->getDimension().z;
    dimensions_.set(ss.str());
    format_.set(volume->getDataFormat()->getString());

    // Use state from volume as default
    dataRange_.setCurrentStateAsDefault();
    valueRange_.setCurrentStateAsDefault();
    valueUnit_.setCurrentStateAsDefault();
    lengths_.setCurrentStateAsDefault();
    angles_.setCurrentStateAsDefault();
    offset_.setCurrentStateAsDefault();
    dimensions_.setCurrentStateAsDefault();
    format_.setCurrentStateAsDefault();

    // If we where deserializing, we just wrote over all the state, now we have to restore it.
    if (isDeserializing_) {
        restoreState();
        overrideLengths_ = lengths_.get();
        overrideAngles_ = angles_.get();
        overrideOffset_ = offset_.get();
    } else {
        // Setup override values. This will trigger onOverrideChange().
        overRideDefaults_.set(false);
    }

    InviwoApplication::getPtr()->getProcessorNetwork()->unlock();
    invalidateOutput();
}

void VolumeSource::saveState(){
    oldState.dataRange.set(&dataRange_);
    oldState.valueRange.set(&valueRange_);
    oldState.valueUnit.set(&valueUnit_);
    oldState.overRideDefaults.set(&overRideDefaults_);
    oldState.lengths.set(&lengths_);
    oldState.angles.set(&angles_);
    oldState.offset.set(&offset_);
}

void VolumeSource::restoreState() {
    // This is more tricky than it seems since, we have saved all the properties, but since we only 
    // de serialize when something changed, we will not have written data in all the saved 
    // properties hence some might still only hold the default state since construction, so we have
    // to check if the state has chaneged before we restore it.

    DoubleMinMaxProperty defaultDataRange("dataRange", "Data range", 0., 255.0, -DataFLOAT64::max(),
                                          DataFLOAT64::max(), 0.0, 0.0,
                                          PropertyOwner::INVALID_OUTPUT, PropertySemantics("Text"));
    DoubleMinMaxProperty defaultValueRange(
        "valueRange", "Value range", 0., 255.0, -DataFLOAT64::max(), DataFLOAT64::max(), 0.0, 0.0,
        PropertyOwner::INVALID_OUTPUT, PropertySemantics("Text"));

    StringProperty defaultValueUnit("valueUnit", "Value unit", "arb. unit.");
    BoolProperty defaultOverRideDefaults("override", "Override", false);
    FloatVec3Property defaultLengths("length", "Lengths", vec3(1.0f), vec3(0.0f), vec3(10.0f));
    FloatVec3Property defaultAngles("angles", "Angles", vec3(90.0f), vec3(0.0f), vec3(180.0f), vec3(1.0f));
    FloatVec3Property defaultOffset("offset", "Offset", vec3(0.0f), vec3(-10.0f), vec3(10.0f));

    defaultDataRange.set(&oldState.dataRange);
    defaultDataRange.resetToDefaultState();
    VolumeSourceState::assignStateIfChanged(oldState.dataRange, defaultDataRange, dataRange_);

    defaultValueRange.set(&oldState.valueRange);
    defaultValueRange.resetToDefaultState();
    VolumeSourceState::assignStateIfChanged(oldState.valueRange, defaultValueRange, valueRange_);

    defaultValueUnit.set(&oldState.valueUnit);
    defaultValueUnit.resetToDefaultState();
    VolumeSourceState::assignStateIfChanged(oldState.valueUnit, defaultValueUnit, valueUnit_);

    defaultOverRideDefaults.set(&oldState.overRideDefaults);
    defaultOverRideDefaults.resetToDefaultState();
    VolumeSourceState::assignStateIfChanged(oldState.overRideDefaults, defaultOverRideDefaults, overRideDefaults_);

    defaultLengths.set(&oldState.lengths);
    defaultLengths.resetToDefaultState();
    VolumeSourceState::assignStateIfChanged(oldState.lengths, defaultLengths, lengths_);

    defaultAngles.set(&oldState.angles);
    defaultAngles.resetToDefaultState();
    VolumeSourceState::assignStateIfChanged(oldState.angles, defaultAngles, angles_);

    defaultOffset.set(&oldState.offset);
    defaultOffset.resetToDefaultState();
    VolumeSourceState::assignStateIfChanged(oldState.offset, defaultOffset, offset_);
}

void VolumeSource::process() {
    Volume* out;

    if (this->isDeserializing_) {
        return;
    }

    if (port_.hasData()) {
        out = port_.getData();

        float a = lengths_.get()[0];
        float b = lengths_.get()[1];
        float c = lengths_.get()[2];
        vec3 offset = offset_.get();
        float alpha = glm::radians(angles_.get()[0]);
        float beta = glm::radians(angles_.get()[1]);
        float gamma = glm::radians(angles_.get()[2]);
        float v = std::sqrt(1 - std::cos(alpha) * std::cos(alpha) -
                            std::cos(beta) * std::cos(beta) - std::cos(gamma) * std::cos(gamma) -
                            2 * std::cos(alpha) * std::cos(beta) * std::cos(gamma));
        mat4 newBasisAndOffset(
            a, b * std::cos(gamma), c * std::cos(beta), offset[0], 0.0f, b * std::sin(gamma),
            c * (std::cos(alpha) - std::cos(beta) * std::cos(gamma)) / std::sin(gamma), offset[1],
            0.0f, 0.0f, c * v / std::sin(gamma), offset[2], 0.0f, 0.0f, 0.0f, 1.0f);
        
        out->setBasisAndOffset(glm::transpose(newBasisAndOffset));
        out->dataMap_.dataRange = dataRange_.get();
        out->dataMap_.valueRange = valueRange_.get();
        out->dataMap_.valueUnit = valueUnit_.get();
    }
}

void VolumeSource::serialize(IvwSerializer& s) const {
    DataSource<Volume, VolumeOutport>::serialize(s);
}

void VolumeSource::deserialize(IvwDeserializer& d) {
    isDeserializing_ = true;
    // This function will deseialize all properties, then call load(), which will call dataLoaded()
    DataSource<Volume, VolumeOutport>::deserialize(d);
    onOverrideChange();
    isDeserializing_ = false;
}

VolumeSource::VolumeSourceState::VolumeSourceState()
    : dataRange("dataRange", "Data range", 0., 255.0, -DataFLOAT64::max(), DataFLOAT64::max(), 0.0,
                0.0, PropertyOwner::INVALID_OUTPUT, PropertySemantics("Text"))
    , valueRange("valueRange", "Value range", 0., 255.0, -DataFLOAT64::max(), DataFLOAT64::max(),
                 0.0, 0.0, PropertyOwner::INVALID_OUTPUT, PropertySemantics("Text"))
    , valueUnit("valueUnit", "Value unit", "arb. unit.")
    , overRideDefaults("override", "Override", false)
    , lengths("length", "Lengths", vec3(1.0f), vec3(0.0f), vec3(10.0f))
    , angles("angles", "Angles", vec3(90.0f), vec3(0.0f), vec3(180.0f), vec3(1.0f))
    , offset("offset", "Offset", vec3(0.0f), vec3(-10.0f), vec3(10.0f)) {}

void VolumeSource::VolumeSourceState::assignStateIfChanged(const DoubleMinMaxProperty& in,
                                                           const DoubleMinMaxProperty& ref,
                                                           DoubleMinMaxProperty& out) {
    if (in.get() != ref.get()) {
        out.set(in.get());
    }
    if (in.getRange() != ref.getRange()) {
        out.setRange(in.getRange());
    }
    if (in.getIncrement() != ref.getIncrement()) {
        out.setIncrement(in.getIncrement());
    }
    if (in.getMinSeparation() != ref.getMinSeparation()) {
        out.setMinSeparation(in.getMinSeparation());
    }
    if (in.getReadOnly() != ref.getReadOnly()) {
        out.setReadOnly(in.getReadOnly());
    }
}

void VolumeSource::VolumeSourceState::assignStateIfChanged(const StringProperty& in,
                                                           const StringProperty& ref,
                                                           StringProperty& out) {
    if (in.get() != ref.get()) {
        out.set(in.get());
    }
    if (in.getReadOnly() != ref.getReadOnly()) {
        out.setReadOnly(in.getReadOnly());
    }
}

void VolumeSource::VolumeSourceState::assignStateIfChanged(const BoolProperty& in,
                                                           const BoolProperty& ref,
                                                           BoolProperty& out) {
    if (in.get() != ref.get()) {
        out.set(in.get());
    }
    if (in.getReadOnly() != ref.getReadOnly()) {
        out.setReadOnly(in.getReadOnly());
    }
}

void VolumeSource::VolumeSourceState::assignStateIfChanged(const FloatVec3Property& in,
                                                           const FloatVec3Property& ref,
                                                           FloatVec3Property& out) {
    if (in.get() != ref.get()) {
        out.set(in.get());
    }
    if (in.getMinValue() != ref.getMinValue()) {
        out.setMinValue(in.getMinValue());
    }
    if (in.getMaxValue() != ref.getMaxValue()) {
        out.setMaxValue(in.getMaxValue());
    }
    if (in.getIncrement() != ref.getIncrement()) {
        out.setIncrement(in.getIncrement());
    }
    if (in.getReadOnly() != ref.getReadOnly()) {
        out.setReadOnly(in.getReadOnly());
    }
}

}  // namespace
