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
#include <inviwo/core/io/imageio.h>

namespace inviwo {

LayerDisk::LayerDisk(LayerType type)
    : LayerRepresentation(uvec2(0), type, DataFormatBase::get()), DiskRepresentation(){
    initialize();
}

LayerDisk::LayerDisk(std::string url, LayerType type)
    : LayerRepresentation(uvec2(0), type, DataFormatBase::get()), DiskRepresentation(url){
    initialize();
}

LayerDisk::LayerDisk(const LayerDisk& rhs) 
    : LayerRepresentation(rhs)
    , DiskRepresentation(rhs){
    initialize();
}

LayerDisk& LayerDisk::operator=(const LayerDisk& that) {
    if(this != &that) {
        LayerRepresentation::operator=(that);
    }
    return *this;
}

LayerDisk::~LayerDisk() {
    deinitialize();
}

LayerDisk* LayerDisk::clone() const {  
    return new LayerDisk(*this);
}

void LayerDisk::initialize(){}

void LayerDisk::deinitialize() {}

std::string LayerDisk::getClassName() const { 
    return "LayerDisk"; 
}

bool LayerDisk::copyAndResizeLayer(DataRepresentation*) const{ 
    return false; 
}

//Hold the requested size of the image, not the actual size
//If the actual image should be loaded without recaling, the dimension need to equal zero
void LayerDisk::resize(uvec2 dim){
    dimensions_ = dim;
}

void* LayerDisk::loadFileData(void* dst, uvec2& outDimension, DataFormatId& outFormatId) const {
    if (hasSourceFile())
        return ImageIO::loadImageToData(dst, getSourceFile(), outDimension, outFormatId);

    return NULL;
}

void* LayerDisk::loadFileDataAndRescale(void* dst, uvec2 dstDimesion, DataFormatId& outFormatId) const {
    if (hasSourceFile())
        return ImageIO::loadImageToDataAndRescale(dst, getSourceFile(), dstDimesion, outFormatId);

    return NULL;
}



} // namespace
