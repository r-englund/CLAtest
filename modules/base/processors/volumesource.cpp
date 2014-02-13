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
#include <inviwo/core/datastructures/volume/volumedisk.h>
#include <inviwo/core/resources/resourcemanager.h>
#include <inviwo/core/resources/templateresource.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>
#include <inviwo/core/util/urlparser.h>
#include <inviwo/core/io/datareaderfactory.h>
#include <inviwo/core/io/rawvolumereader.h>
#include <math.h>

namespace inviwo {

ProcessorClassName(VolumeSource, "VolumeSource"); 
ProcessorCategory(VolumeSource, "Data Input");
ProcessorCodeState(VolumeSource, CODE_STATE_EXPERIMENTAL);

VolumeSource::VolumeSource()
    : Processor()
    , volumePort_("volume")
	, volumeFile_("volumeFileName", "Volume file")
    , dataRange_("dataRange", "Data range", ivec2(0,255), ivec2(0,0), ivec2(255,255))
    , valueRange_("valueRange", "Value range", vec2(0.0f,1.0f), vec2(0.0f,0.0f), vec2(1.0f,1.0f))
    , valueUnit_("valueUnit", "Value unit", "")
    , rawFileName_("rawFileName", "", "") // the following properties are hidden
    , rawFormatStr_("rawFormatStr", "", "")
    , rawDims_("rawDims", "", ivec3(0,0,0))
    , rawEndianess_("rawEndianess", "", true)
{
    
    addPort(volumePort_);

	volumeFile_.onChange(this, &VolumeSource::loadVolume);
    
    std::vector<FileExtension> ext = DataReaderFactory::getRef().getExtensionsForType<Volume>();
    for(std::vector<FileExtension>::const_iterator it = ext.begin();
        it != ext.end(); ++it) {
            std::stringstream ss;
            ss << it->description_ << " (*." << it->extension_ << ")";
            volumeFile_.addNameFilter(ss.str());
    }

	addProperty(volumeFile_);
    dataRange_.onChange(this, &VolumeSource::invalidateOutput);
    addProperty(dataRange_);
    valueRange_.onChange(this, &VolumeSource::invalidateOutput);
    addProperty(valueRange_);
    valueUnit_.onChange(this, &VolumeSource::invalidateOutput);
    addProperty(valueUnit_);

    // add the properties used to serialize the state of the raw reader
    rawFileName_.setVisibility(INVISIBLE);
    addProperty(rawFileName_);
    rawFormatStr_.setVisibility(INVISIBLE);
    addProperty(rawFormatStr_);
    rawDims_.setVisibility(INVISIBLE);
    addProperty(rawDims_);
    rawEndianess_.setVisibility(INVISIBLE);
    addProperty(rawEndianess_);
}

VolumeSource::~VolumeSource() {}

bool VolumeSource::isReady() const {
    return URLParser::fileExists(volumeFile_.get());
}

void VolumeSource::invalidateOutput() {
    Volume* volume = volumePort_.getData();
    if (volume) {
        volume->setMetaData<IVec2MetaData>("DataRange", dataRange_.get());
        volume->setMetaData<Vec2MetaData>("ValueRange", valueRange_.get());
        volume->setMetaData<StringMetaData>("ValueUnit", valueUnit_.get());
    }
    volumePort_.invalidate(PropertyOwner::INVALID_OUTPUT);
}

void VolumeSource::updateRangeProperties(Volume* volume) {
    int numValues = static_cast<int>(pow(2.0f, static_cast<float>(volume->getDataFormat()->getBitsStored())));
    dataRange_.setMaxValue(ivec2(numValues-1, numValues-1));
    if (volume->hasMetaData<IVec2MetaData>("DataRange"))
        dataRange_.set(volume->getMetaData<IVec2MetaData>("DataRange", dataRange_.get()));
    else
        dataRange_.set(ivec2(0, numValues-1));
    valueRange_.set(volume->getMetaData<Vec2MetaData>("ValueRange", valueRange_.get()));
    valueUnit_.set(volume->getMetaData<StringMetaData>("ValueUnit", valueUnit_.get()));
    invalidateOutput();
}

void VolumeSource::loadVolume() {
    TemplateResource<Volume>* volumeResource = ResourceManager::instance()->getResourceAs<TemplateResource<Volume> >(volumeFile_.get());
    if (volumeResource) {
        volumePort_.setData(volumeResource->getData(), false);
        updateRangeProperties(volumeResource->getData());
    } else {
        std::string fileExtension = URLParser::getFileExtension(volumeFile_.get());
        DataReaderType<Volume>* reader = DataReaderFactory::getRef().getReaderForTypeAndExtension<Volume>(fileExtension);
        if (reader) {
            try {
                Volume* volume = reader->readMetaData(volumeFile_.get());
                ResourceManager::instance()->addResource(new TemplateResource<Volume>(volumeFile_.get(), volume));
                volumePort_.setData(volume, false);
                updateRangeProperties(volume);
            } catch(DataReaderException const& e) {
                LogError(e.getMessage());
            }
        } else {
            LogError("Could not load volume: " << volumeFile_.get());
        }
    }
}

} // namespace
