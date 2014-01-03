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

#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/datastructures/image/imageramprecision.h>
#include <inviwo/core/io/imageio.h>

namespace inviwo {

ImageRAM::ImageRAM(uvec2 dimension, ImageType type, const DataFormatBase* format)
    : ImageRepresentation(dimension, type, format)
{
    ImageRAM::initialize();
}

ImageRAM::~ImageRAM() {
    ImageRAM::deinitialize();
}  

void ImageRAM::initialize() {
    data_ = NULL;
    depthData_ = NULL;
    pickingData_ = NULL;
}

void ImageRAM::deinitialize() {
    // Make sure that data is deinitialized in
    // child class (should not delete void pointer 
    // since destructor will not be called for object).
    if(depthData_) {
        delete[] depthData_;
        depthData_ = NULL;
    }
}

float* ImageRAM::getDepthData() {
    if (!depthData_)
        allocateDepthData();

    return depthData_;
}

void* ImageRAM::getPickingData() {
    if(!pickingData_)
        allocatePickingData();

    return pickingData_;
}

bool ImageRAM::copyAndResizeImage(DataRepresentation* targetImageRam) {
    ImageRAM* source = this;
    ImageRAM* target = dynamic_cast<ImageRAM*>(targetImageRam);
    ivwAssert(target!=0, "Target representation missing.");

    //CPU image rescaling using image loader
    uvec2 targetDimensions  = target->getDimensions();
    void* rawData = ImageIO::rescaleImageRAM(source, targetDimensions.x, targetDimensions.y);

    if (!rawData) return false;

    target->setData(rawData);

    return true;
}

void ImageRAM::setDimensions( uvec2 dimensions )
{
    resize(dimensions);
}

float ImageRAM::getDepthValue(const uvec2& pos) const{
    if (depthData_)
        return depthData_[posToIndex(pos, dimensions_)];
    else
        return 1.f;
}

void ImageRAM::allocateDepthData(){
    depthData_ = new float[dimensions_.x*dimensions_.y];
}

ImageRAM* createImageRAM(const uvec2& dimension, ImageType type, const DataFormatBase* format) {
    switch (format->getId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("createImageRAM", "Invalid format");
        return NULL;
    #define DataFormatIdMacro(i) case i: return new ImageRAMCustomPrecision<Data##i::type, Data##i::bits>(dimension, type); break;
    #include <inviwo/core/util/formatsdefinefunc.h>
    default:
        LogErrorCustom("createImageRAM", "Invalid format or not implemented");
        return NULL;
    }
    return NULL;
}

} // namespace
