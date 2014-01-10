/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include "imagesourceseries.h"
#include <inviwo/core/io/imageio.h>
#include <inviwo/core/util/filedirectory.h>
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/datastructures/image/layerdisk.h>

namespace inviwo {

ProcessorClassName(ImageSourceSeries, "ImageSourceSeries"); 
ProcessorCategory(ImageSourceSeries, "Data Input");
ProcessorCodeState(ImageSourceSeries, CODE_STATE_EXPERIMENTAL);

ImageSourceSeries::ImageSourceSeries()
    : Processor()
    , outport_("image.outport")
    , findFilesButton_("findFiles", "Find Files")
    , imageFileDirectory_("imageFileDirectory", "Image file directory", IVW_DIR+"data/images")
    , currentImageIndex_("currentImageIndex", "Image index", 1, 1, 1, 1){

    addPort(outport_);
    addProperty(imageFileDirectory_);
    addProperty(findFilesButton_);
    addProperty(currentImageIndex_);

    imageFileDirectory_.registerFileIndexingHandle(&currentImageIndex_);
    findFilesButton_.onChange(this, &ImageSourceSeries::onFindFiles);
}

ImageSourceSeries::~ImageSourceSeries() {}

void ImageSourceSeries::initialize() {
    Processor::initialize();
}

void ImageSourceSeries::deinitialize() {
    Processor::deinitialize();
}

void ImageSourceSeries::onFindFiles() {
    std::vector<std::string> files = imageFileDirectory_.getFiles();         

    //imageFilesInDirectory_.clearOptions();
    std::vector<std::string> ids;
    std::vector<std::string> displayNames;
    for (size_t i=0; i<files.size(); i++) {
        if (ImageIO::isValidImageFile(files[i]) ) {
            std::string displayName = URLParser::getFileNameWithExtension(files[i]);
            ids.push_back(displayName+"_id");
            displayNames.push_back(displayName);
        }
    }

    //TODO: following settings leads to crash. under investigation.
    if(ids.size())
        currentImageIndex_.setMaxValue(static_cast<const int>(ids.size()));

    if(!ids.size()) {
        ids.push_back("noImage");
        displayNames.push_back("noImage");
        files.push_back("");
    }
 
    currentImageIndex_.set(1);
}

/**
* Creates a ImageDisk representation if there isn't an object already defined.
**/
void ImageSourceSeries::process() {
	Image* outImage = outport_.getData(); 
    if (outImage){
        
        std::vector<std::string> filesInDirectory = imageFileDirectory_.getFiles();
        std::vector<std::string> fileNames;
        for (size_t i=0; i<filesInDirectory.size(); i++) {
            if (ImageIO::isValidImageFile(filesInDirectory[i]) ) {                
                fileNames.push_back(filesInDirectory[i]);
            }
        }

        size_t currentIndex = currentImageIndex_.get();

        if (!fileNames.size()) {
            LogWarn("No image found in the directory.");
            return;
        }

        if (currentIndex > fileNames.size()) {
            LogWarn("Current index exceeded the number of files.");
            return;
        }

        if (currentIndex==0) {
            LogWarn("Invalid index");
            return;
        }

        std::string currentFileName = fileNames[currentIndex-1];
        LayerDisk* outLayerDisk = outImage->getColorLayer()->getEditableRepresentation<LayerDisk>();
        if (!outLayerDisk || outLayerDisk->getSourceFile() != currentFileName){ 
            outLayerDisk = new LayerDisk(currentFileName);
            outImage->getColorLayer()->clearRepresentations();
            outImage->getColorLayer()->addRepresentation(outLayerDisk);
        }

        //Original image dimension loaded from disk may differ from requested dimension.
       outLayerDisk->resize(outImage->getDimension());
    }
}

} // namespace
