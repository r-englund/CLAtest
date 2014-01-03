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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/io/imageio.h>

namespace inviwo {

ImageDisk::ImageDisk()
    : ImageRepresentation(uvec2(0), COLOR_DEPTH_PICKING, DataFormatBase::get()), DiskRepresentation(){}

ImageDisk::ImageDisk(std::string url)
    : ImageRepresentation(uvec2(0), COLOR_DEPTH_PICKING, DataFormatBase::get()), DiskRepresentation(url){
    initialize();
}

ImageDisk::~ImageDisk() {
}

void ImageDisk::initialize(){
}

DataFormatId ImageDisk::loadFileData(void* dst) const {
    if (hasSourceFile())
        if(dimensions_.x > 0 && dimensions_.y > 0)
            return ImageIO::loadImageToDataAndRescale(dst, getSourceFile(), dimensions_.x, dimensions_.y);
        else
            return ImageIO::loadImageToData(dst, getSourceFile());

    return NOT_SPECIALIZED;
}

DataFormatId ImageDisk::loadFileDataAndRescale(void* dst, uvec2 dst_dimesion) const {
    if (hasSourceFile())
        return ImageIO::loadImageToDataAndRescale(dst, getSourceFile(), dst_dimesion.x, dst_dimesion.y);

    return NOT_SPECIALIZED;
}

void ImageDisk::deinitialize() {}

void ImageDisk::resize(uvec2 dimensions){        
    dimensions_ = dimensions;
} 

ImageDisk* ImageDisk::clone() const {
    //TODO: move to copyconstructor
    ImageDisk* imageDiskClone = new ImageDisk(getSourceFile());
    imageDiskClone->resize(dimensions_);
    return imageDiskClone;
}

} // namespace
