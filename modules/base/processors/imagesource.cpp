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
 * Contact: Erik Sundén
 *
 *********************************************************************************/

#include "imagesource.h"
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/datastructures/image/layerdisk.h>
#include <inviwo/core/io/datareaderfactory.h>
#include <inviwo/core/util/filesystem.h>

namespace inviwo {

ProcessorClassIdentifier(ImageSource, "org.inviwo.ImageSource");
ProcessorDisplayName(ImageSource,  "Image Source");
ProcessorTags(ImageSource, Tags::None);
ProcessorCategory(ImageSource, "Data Input");
ProcessorCodeState(ImageSource, CODE_STATE_EXPERIMENTAL);

ImageSource::ImageSource()
    : Processor(),
     outport_("image.outport", COLOR_ONLY, DataVec4UINT8::get(), PropertyOwner::INVALID_OUTPUT, false),
     imageFileName_("imageFileName", "Image file name", "" , "image"),
     isDeserializing_(false)
{
    addPort(outport_);

    imageFileName_.onChange(this, &ImageSource::load);
    std::vector<FileExtension> ext = DataReaderFactory::getPtr()->getExtensionsForType<Layer>();
    for (std::vector<FileExtension>::const_iterator it = ext.begin(); it != ext.end(); ++it) {
        std::stringstream ss;
        ss << it->description_ << " (*." << it->extension_ << ")";
        imageFileName_.addNameFilter(ss.str());
    }

    addProperty(imageFileName_);
}

ImageSource::~ImageSource() {}

void ImageSource::initialize() {
    Processor::initialize();
}

void ImageSource::deinitialize() {
    Processor::deinitialize();
}

bool ImageSource::isReady() const {
    return filesystem::fileExists(imageFileName_.get());
}

void ImageSource::process() {}

void ImageSource::load() {
    if (isDeserializing_ || imageFileName_.get() == "") {
        return;
    }

    std::string fileExtension = filesystem::getFileExtension(imageFileName_.get());
    DataReaderType<Layer>* reader = DataReaderFactory::getPtr()->getReaderForTypeAndExtension<Layer>(fileExtension);

    if (reader) {
        try {
            Layer* outLayer = reader->readMetaData(imageFileName_.get());
            Image* outImage = new Image(outLayer);
            outport_.setData(outImage);
        }
        catch (DataReaderException const& e) {
            LogError("Could not load data: " << imageFileName_.get() << ", " << e.getMessage());
            imageFileName_.set("");
        }
    } else {
        LogError("Could not find a data reader for file: " << imageFileName_.get());
        imageFileName_.set("");
    }
}

/**
 * Deserialize everything first then load the data
 */
void ImageSource::deserialize(IvwDeserializer& d) {
    isDeserializing_ = true;
    Processor::deserialize(d);
    isDeserializing_ = false;
    load();
}

} // namespace
