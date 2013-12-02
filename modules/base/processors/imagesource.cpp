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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include "imagesource.h"

namespace inviwo {

ProcessorClassName(ImageSource, "ImageSource"); 
ProcessorCategory(ImageSource, "Data Input");
ProcessorCodeState(ImageSource, CODE_STATE_EXPERIMENTAL);

ImageSource::ImageSource()
    : Processor(),
    outport_("image.outport"),
    imageFileName_("imageFileName", "Image file name", IVW_DIR+"data/images/swirl.tga")
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

/**
* Creates a ImageDisk representation if there isn't an object already defined.
**/
void ImageSource::process() {
	Image* outImage = outport_.getData(); 
    if (outImage){
                
        ImageDisk* outImageDisk = outImage->getEditableRepresentation<ImageDisk>();
        if (!outImageDisk || outImageDisk->getSourceFile() != imageFileName_.get()){ 
            outImageDisk = new ImageDisk(imageFileName_.get());
            outImage->clearRepresentations();
            outImage->addRepresentation(outImageDisk);
        }

        //Original image dimension loaded from disk may differ from requested dimension.
        outImageDisk->resize(outImage->getDimension());
    }
}

} // namespace
