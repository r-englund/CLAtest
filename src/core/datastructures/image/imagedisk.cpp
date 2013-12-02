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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/io/imageloader.h>

namespace inviwo {

    ImageDisk::ImageDisk()
        : ImageRepresentation(uvec2(256,256), COLOR_DEPTH_PICKING, DataVec4UINT8::get()), DiskRepresentation(){}

	ImageDisk::ImageDisk(std::string url)
        : ImageRepresentation(uvec2(256,256), COLOR_DEPTH_PICKING, DataVec4UINT8::get()), DiskRepresentation(url){
        initialize();
	}

    ImageDisk::~ImageDisk() {
    }

    void ImageDisk::initialize(){
        if (hasSourceFile()){
            dimensions_ = ImageLoader::imageDimensions(getSourceFile());
        }
    }

    void* ImageDisk::loadFileData() const {
        if (hasSourceFile())            
            return ImageLoader::loadImageToDataAndRescale(getSourceFile(), dimensions_.x, dimensions_.y);

        return NULL;
    }

    void* ImageDisk::loadFileDataAndRescale(uvec2 dst_dimesion) const {
        if (hasSourceFile())
            return ImageLoader::loadImageToDataAndRescale(getSourceFile(), dst_dimesion.x, dst_dimesion.y);

        return NULL;
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
