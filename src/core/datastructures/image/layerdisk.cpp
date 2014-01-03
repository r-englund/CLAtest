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

#include <inviwo/core/datastructures/image/layerdisk.h>
#include <inviwo/core/io/imageloader.h>

namespace inviwo {

LayerDisk::LayerDisk()
    : LayerRepresentation(uvec2(0), DataFormatBase::get()), DiskRepresentation(){}

LayerDisk::LayerDisk(std::string url)
    : LayerRepresentation(uvec2(0), DataFormatBase::get()), DiskRepresentation(url){
    initialize();
}

LayerDisk::~LayerDisk() {
}

void LayerDisk::initialize(){
}

DataFormatId LayerDisk::loadFileData(void* dst) const {
    if (hasSourceFile())
        if(dimensions_.x > 0 && dimensions_.y > 0)
            return ImageLoader::loadImageToDataAndRescale(dst, getSourceFile(), dimensions_.x, dimensions_.y);
        else
            return ImageLoader::loadImageToData(dst, getSourceFile());

    return NOT_SPECIALIZED;
}

DataFormatId LayerDisk::loadFileDataAndRescale(void* dst, uvec2 dst_dimesion) const {
    if (hasSourceFile())
        return ImageLoader::loadImageToDataAndRescale(dst, getSourceFile(), dst_dimesion.x, dst_dimesion.y);

    return NOT_SPECIALIZED;
}

void LayerDisk::deinitialize() {}

void LayerDisk::resize(uvec2 dimensions){        
    dimensions_ = dimensions;
} 

LayerDisk* LayerDisk::clone() const {
    //TODO: move to copy constructor
    LayerDisk* imageDiskClone = new LayerDisk(getSourceFile());
    imageDiskClone->resize(dimensions_);
    return imageDiskClone;
}

} // namespace
