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
#include <math.h>

namespace inviwo {

ProcessorClassName(VolumeSource, "VolumeSource"); 
ProcessorCategory(VolumeSource, "Data Input");
ProcessorCodeState(VolumeSource, CODE_STATE_EXPERIMENTAL);

VolumeSource::VolumeSource()
    : DataSource<Volume, VolumeOutport>()
    , dataRange_("dataRange", "Data range", 0.f, 255.0f, 0.f, 255.0f)
    , valueRange_("valueRange", "Value range", 0.f, 255.0f, 0.f, 255.0f)
    , valueUnit_("valueUnit", "Value unit", "") {
    
    DataSource<Volume, VolumeOutport>::file_.setDisplayName("Volume file");

    dataRange_.onChange(this, &VolumeSource::invalidateOutput);
    addProperty(dataRange_);
    valueRange_.onChange(this, &VolumeSource::invalidateOutput);
    addProperty(valueRange_);
    valueUnit_.onChange(this, &VolumeSource::invalidateOutput);
    addProperty(valueUnit_);
}

VolumeSource::~VolumeSource() {}

void VolumeSource::invalidateOutput() {
    Volume* volume = DataSource<Volume, VolumeOutport>::port_.getData();
    if (volume) {
        volume->setMetaData<Vec2MetaData>("DataRange", dataRange_.get());
        volume->setMetaData<Vec2MetaData>("ValueRange", valueRange_.get());
        volume->setMetaData<StringMetaData>("ValueUnit", valueUnit_.get());
    }
    DataSource<Volume, VolumeOutport>::invalidateOutput();
}

void VolumeSource::dataLoaded(Volume* volume) {
    float max = static_cast<float>(volume->getDataFormat()->getMax());
    float min = static_cast<float>(volume->getDataFormat()->getMin());

    dataRange_.setRangeMin(min);
    dataRange_.setRangeMax(max);

    if(volume->hasMetaData<DVec2MetaData>("DataRange")) {
        dataRange_.set(volume->getMetaData<Vec2MetaData>("DataRange", dataRange_.get()));
    }else{
        dataRange_.set(vec2(min, max));
    }
    valueRange_.set(volume->getMetaData<Vec2MetaData>("ValueRange", dataRange_.get()));
    valueUnit_.set(volume->getMetaData<StringMetaData>("ValueUnit", valueUnit_.get()));
    invalidateOutput();
}


} // namespace
