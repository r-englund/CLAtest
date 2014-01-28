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
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#include "volumesource.h"
#include <inviwo/core/datastructures/volume/volumedisk.h>
#include <inviwo/core/resources/resourcemanager.h>
#include <inviwo/core/resources/templateresource.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>
#include <inviwo/core/util/urlparser.h>
#include <inviwo/core/io/datareaderfactory.h>
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
{
    addPort(volumePort_);

	volumeFile_.onChange(this, &VolumeSource::loadVolume);
    
    std::vector<FileExtension> ext = DataReaderFactory::getRef().getExtensionsForType<Volume>();
    for(std::vector<FileExtension>::const_iterator it = ext.begin();
        it != ext.end(); ++it){
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
}

VolumeSource::~VolumeSource() {}

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
