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
 * Main file authors: Erik Sundén, Timo Ropinski
 *
 *********************************************************************************/

#include "imagesource.h"
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/datastructures/image/layerdisk.h>

namespace inviwo {

ProcessorClassName(ImageSource, "ImageSource");
ProcessorCategory(ImageSource, "Data Input");
ProcessorCodeState(ImageSource, CODE_STATE_EXPERIMENTAL);

ImageSource::ImageSource()
    : Processor(),
    outport_("image.outport"),
    imageFileName_("imageFileName", "Image file name", "")
{
    addPort(outport_);
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
    return (imageFileName_.get() != "");
}

/**
* Creates a ImageDisk representation if there isn't an object already defined.
**/
void ImageSource::process() {
	Image* outImage = outport_.getData();
    if (outImage){

        LayerDisk* outLayerDisk = outImage->getColorLayer()->getEditableRepresentation<LayerDisk>();
        if (!outLayerDisk || outLayerDisk->getSourceFile() != imageFileName_.get()){
            outLayerDisk = new LayerDisk(imageFileName_.get());
            outImage->getColorLayer()->clearRepresentations();
            outImage->getColorLayer()->addRepresentation(outLayerDisk);
        }

        //Original image dimension loaded from disk may differ from requested dimension.
        outLayerDisk->resize(outImage->getDimension());
    }
}

} // namespace
