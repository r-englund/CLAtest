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
 * Contact: Peter Steneteg
 *
 *********************************************************************************/

#include "volumesource.h"
#include <inviwo/core/resources/resourcemanager.h>
#include <inviwo/core/resources/templateresource.h>
#include <inviwo/core/util/urlparser.h>
#include <inviwo/core/io/datareaderfactory.h>
#include <inviwo/core/io/rawvolumereader.h>
#include <inviwo/core/network/processornetwork.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <glm/gtx/vector_angle.hpp>
#include <math.h>

namespace inviwo {

ProcessorClassIdentifier(VolumeSource, "org.inviwo.VolumeSource");
ProcessorDisplayName(VolumeSource,  "Volume Source");
ProcessorTags(VolumeSource, Tags::None);
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
    , a_("a", "A", vec3(1.0f, 0.0f, 0.0f), vec3(-10.0f), vec3(10.0f))
    , b_("b", "B", vec3(0.0f, 1.0f, 0.0f), vec3(-10.0f), vec3(10.0f))
    , c_("c", "C", vec3(0.0f, 0.0f, 1.0f), vec3(-10.0f), vec3(10.0f))
    , offset_("offset", "Offset", vec3(0.0f), vec3(-10.0f), vec3(10.0f))
    , selectedSequenceIndex_("selectedSequenceIndex", "Selected Sequence Index", 1, 1, 1, 1, PropertyOwner::VALID)
    , playSequence_("playSequence", "Play Sequence", false)
    , volumesPerSecond_("volumesPerSecond", "Volumes Per Second", 30, 1, 60, 1, PropertyOwner::VALID)
    , dimensions_("dimensions", "Dimensions")
    , format_("format", "Format", "")
    , basis_("Basis", "Basis and offset")
    , information_("Information", "Data information")
    , isDeserializing_(false)
    , sequenceTimer_(NULL) {

    DataSource<Volume, VolumeOutport>::file_.setContentType("volume");
    DataSource<Volume, VolumeOutport>::file_.setDisplayName("Volume file");

    a_.setReadOnly(true);
    b_.setReadOnly(true);
    c_.setReadOnly(true);
    offset_.setReadOnly(true);

    dimensions_.setReadOnly(true);
    format_.setReadOnly(true);
    dimensions_.setCurrentStateAsDefault();
    format_.setCurrentStateAsDefault();

    overrideA_ = a_.get();
    overrideB_ = b_.get();
    overrideC_ = c_.get();
    overrideOffset_ = offset_.get();

    overRideDefaults_.onChange(this, &VolumeSource::onOverrideChange);

    information_.addProperty(dimensions_);
    information_.addProperty(format_);
    information_.addProperty(dataRange_);
    information_.addProperty(valueRange_);
    information_.addProperty(valueUnit_);

    addProperty(information_);

    basis_.addProperty(overRideDefaults_);
    basis_.addProperty(a_);
    basis_.addProperty(b_);
    basis_.addProperty(c_);
    basis_.addProperty(offset_);

	addProperty(basis_);
    addProperty(playSequence_);
    playSequence_.setVisible(false);
    playSequence_.onChange(this, &VolumeSource::onPlaySequenceToggled);

    addProperty(selectedSequenceIndex_);
    selectedSequenceIndex_.setVisible(false);
    selectedSequenceIndex_.onChange(this, &VolumeSource::onSequenceIndexChanged);

    addProperty(volumesPerSecond_);
    volumesPerSecond_.setVisible(false);

    sequenceTimer_ = InviwoApplication::getPtr()->createTimer();
    sequenceTimer_->setElapsedTimeCallback(this, &VolumeSource::onSequenceTimerEvent);
}

VolumeSource::~VolumeSource() {
    delete sequenceTimer_;
    sequenceTimer_ = 0;
}

void VolumeSource::onOverrideChange() {
    if (this->isDeserializing_) {
        return;
    }

    if (!overRideDefaults_.get()) {
        overrideA_ = a_.get();
        overrideB_ = b_.get();
        overrideC_ = c_.get();
        overrideOffset_ = offset_.get();
        a_.resetToDefaultState();
        b_.resetToDefaultState();
        c_.resetToDefaultState();
        offset_.resetToDefaultState();
        a_.setReadOnly(true);
        b_.setReadOnly(true);
        c_.setReadOnly(true);
        offset_.setReadOnly(true);
    } else {
        a_.set(overrideA_);
        b_.set(overrideB_);
        c_.set(overrideC_);
        offset_.set(overrideOffset_);
        
        a_.setReadOnly(false);
        b_.setReadOnly(false);
        c_.setReadOnly(false);
        offset_.setReadOnly(false);
    }
}

void VolumeSource::dataLoaded(Volume* volume) {
    InviwoApplication::getPtr()->getProcessorNetwork()->lock();
    
    // Save the old state, used when we have deserialized to be able to restore deserialized values
    // after the file loading have read in the default values.
    saveState();

    // Set the data range from the volume
    dataRange_.set(volume->dataMap_.dataRange);
    valueRange_.set(volume->dataMap_.valueRange);
    valueUnit_.set(volume->dataMap_.valueUnit);
    
    // calculate and set properties basis properties.
    a_.set(volume->getBasis()[0]);
    b_.set(volume->getBasis()[1]);
    c_.set(volume->getBasis()[2]);
    offset_.set(volume->getOffset());
    
    // Display the format and dimension, read only.
    std::stringstream ss;
    ss << volume->getDimension().x << " x "
       << volume->getDimension().y << " x "
       << volume->getDimension().z;

    DataSequence<Volume>* volumeSequence = dynamic_cast<DataSequence<Volume>*>(volume);
    if(volumeSequence){
        ss << " x " << volumeSequence->getNumSequences();
        playSequence_.setVisible(true);
        selectedSequenceIndex_.setVisible(true);
        volumesPerSecond_.setVisible(true);
        selectedSequenceIndex_.setMaxValue(static_cast<int>(volumeSequence->getNumSequences()));
        selectedSequenceIndex_.set(1);
        onPlaySequenceToggled();
    }
    else{
        playSequence_.setVisible(false);
        selectedSequenceIndex_.setVisible(false);
        volumesPerSecond_.setVisible(false);
    }

    dimensions_.set(ss.str());
    format_.set(volume->getDataFormat()->getString());

    // Use state from volume as default
    dataRange_.setCurrentStateAsDefault();
    valueRange_.setCurrentStateAsDefault();
    valueUnit_.setCurrentStateAsDefault();
    a_.setCurrentStateAsDefault();
    b_.setCurrentStateAsDefault();
    c_.setCurrentStateAsDefault();
    offset_.setCurrentStateAsDefault();
    dimensions_.setCurrentStateAsDefault();
    format_.setCurrentStateAsDefault();

    // If we were deserializing, we just wrote over all the state, now we have to restore it.
    if (isDeserializing_) {
        restoreState();
        overrideA_ = a_.get();
        overrideB_ = b_.get();
        overrideC_ = b_.get();
        overrideOffset_ = offset_.get();
    } else {
        // Setup override values. This will trigger onOverrideChange().
        overRideDefaults_.set(false);
    }

    InviwoApplication::getPtr()->getProcessorNetwork()->unlock();
    invalidateOutput();
}

void VolumeSource::onPlaySequenceToggled() {
    if (port_.hasDataSequence()) {
        if(playSequence_.get()){
            sequenceTimer_->start(1000/volumesPerSecond_.get());
            selectedSequenceIndex_.setReadOnly(true);
            volumesPerSecond_.setReadOnly(false);
        }
        else{
            sequenceTimer_->stop();
            selectedSequenceIndex_.setReadOnly(false);
            volumesPerSecond_.setReadOnly(true);
        }
    }
}

void VolumeSource::onSequenceIndexChanged() {
    if (port_.hasDataSequence()) {
        DataSequence<Volume>* volumeSequence = static_cast<DataSequence<Volume>*>(loadedData_);
        volumeSequence->setCurrentIndex(selectedSequenceIndex_.get()-1);
        invalidateOutput();
    }
}

void VolumeSource::onSequenceTimerEvent() {
    if (port_.hasDataSequence()) {
        sequenceTimer_->stop();
        sequenceTimer_->start(1000/volumesPerSecond_.get());
        selectedSequenceIndex_.set((selectedSequenceIndex_.get() < selectedSequenceIndex_.getMaxValue() ? selectedSequenceIndex_.get()+1 : 1));
    }
}

void VolumeSource::saveState(){
    oldState.dataRange.set(&dataRange_);
    oldState.valueRange.set(&valueRange_);
    oldState.valueUnit.set(&valueUnit_);
    oldState.overRideDefaults.set(&overRideDefaults_);
    oldState.a.set(&a_);
    oldState.b.set(&b_);
    oldState.c.set(&c_);
    oldState.offset.set(&offset_);
}

void VolumeSource::restoreState() {
    // This is more tricky than it seems since, we have saved all the properties, but since we only 
    // deserialize when something changed, we will not have written data in all the saved 
    // properties hence some might still only hold the default state since construction, so we have
    // to check if the state has changed before we restore it.

    DoubleMinMaxProperty defaultDataRange("dataRange", "Data range", 0., 255.0, -DataFLOAT64::max(),
                                          DataFLOAT64::max(), 0.0, 0.0,
                                          PropertyOwner::INVALID_OUTPUT, PropertySemantics("Text"));
    DoubleMinMaxProperty defaultValueRange(
        "valueRange", "Value range", 0., 255.0, -DataFLOAT64::max(), DataFLOAT64::max(), 0.0, 0.0,
        PropertyOwner::INVALID_OUTPUT, PropertySemantics("Text"));

    StringProperty defaultValueUnit("valueUnit", "Value unit", "arb. unit.");
    BoolProperty defaultOverRideDefaults("override", "Override", false);
    FloatVec3Property defaultA("a", "a", vec3(1.0f, 0.0f, 0.0f), vec3(-10.0f), vec3(10.0f));
    FloatVec3Property defaultB("b", "b", vec3(0.0f, 1.0f, 0.0f), vec3(-10.0f), vec3(10.0f));
    FloatVec3Property defaultC("c", "c", vec3(0.0f, 0.0f, 1.0f), vec3(-10.0f), vec3(10.0f));
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

    defaultA.set(&oldState.a);
    defaultA.resetToDefaultState();
    VolumeSourceState::assignStateIfChanged(oldState.a, defaultA, a_);

    defaultB.set(&oldState.b);
    defaultB.resetToDefaultState();
    VolumeSourceState::assignStateIfChanged(oldState.b, defaultB, b_);
    
    defaultC.set(&oldState.c);
    defaultC.resetToDefaultState();
    VolumeSourceState::assignStateIfChanged(oldState.c, defaultC, c_);

    defaultOffset.set(&oldState.offset);
    defaultOffset.resetToDefaultState();
    VolumeSourceState::assignStateIfChanged(oldState.offset, defaultOffset, offset_);
}

void VolumeSource::process() {
    Volume* out;

    if (this->isDeserializing_) {
        return;
    }

    if (loadedData_) {
        out = static_cast<Volume*>(loadedData_);

        vec3 a = a_.get();
        vec3 b = b_.get();
        vec3 c = c_.get();
        vec4 offset = vec4(offset_.get(), 1.0f);
        mat3 basis(a,b,c);
        
        mat4 basisAndOffset(basis);
        basisAndOffset[3] = offset;

        out->setBasisAndOffset(basisAndOffset);
        
        if (out->dataMap_.dataRange != dataRange_.get() && out->hasRepresentation<VolumeRAM>()) {
            VolumeRAM* volumeRAM = out->getEditableRepresentation<VolumeRAM>();
            if (volumeRAM->hasNormalizedHistogram()) {
                volumeRAM->getNormalizedHistogram()->setValid(false);
            }
        }
        
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
    , a("a", "a", vec3(1.0f, 0.0f, 0.0f), vec3(-10.0f), vec3(10.0f))
    , b("b", "b", vec3(0.0f, 1.0f, 0.0f), vec3(-10.0f), vec3(10.0f))
    , c("c", "c", vec3(0.0f, 0.0f, 1.0f), vec3(-10.0f), vec3(10.0f))
    , offset("offset", "Offset", vec3(0.0f), vec3(-10.0f), vec3(10.0f)) {
}

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
