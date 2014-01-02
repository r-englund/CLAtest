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
    : LayerRepresentation(uvec2(0), DataVec4UINT8::get()), DiskRepresentation(){}

LayerDisk::LayerDisk(std::string url)
    : LayerRepresentation(uvec2(0), DataVec4UINT8::get()), DiskRepresentation(url){
    initialize();
}

LayerDisk::~LayerDisk() {
}

void LayerDisk::initialize(){
}

void* LayerDisk::loadFileData() const {
    if (hasSourceFile())            
        if(dimensions_.x > 0 && dimensions_.y > 0)
            return ImageLoader::loadImageToDataAndRescale(getSourceFile(), dimensions_.x, dimensions_.y);
        else
            return ImageLoader::loadImageToData(getSourceFile());

    return NULL;
}

void* LayerDisk::loadFileDataAndRescale(uvec2 dst_dimesion) const {
    if (hasSourceFile())
        return ImageLoader::loadImageToDataAndRescale(getSourceFile(), dst_dimesion.x, dst_dimesion.y);

    return NULL;
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
