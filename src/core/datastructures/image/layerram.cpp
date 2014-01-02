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

#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/datastructures/image/layerramprecision.h>
#include <inviwo/core/io/imageloader.h>

namespace inviwo {

LayerRAM::LayerRAM(uvec2 dimension, const DataFormatBase* format)
    : LayerRepresentation(dimension, format)
{
    LayerRAM::initialize();
}

LayerRAM::~LayerRAM() {
    LayerRAM::deinitialize();
}  

void LayerRAM::initialize() {
    data_ = NULL;
    depthData_ = NULL;
    pickingData_ = NULL;
}

void LayerRAM::deinitialize() {
    // Make sure that data is deinitialized in
    // child class (should not delete void pointer 
    // since destructor will not be called for object).
    if(depthData_) {
        delete[] depthData_;
        depthData_ = NULL;
    }
}

float* LayerRAM::getDepthData() {
    if (!depthData_)
        allocateDepthData();

    return depthData_;
}

void* LayerRAM::getPickingData() {
    if(!pickingData_)
        allocatePickingData();

    return pickingData_;
}

bool LayerRAM::copyAndResizeLayer(DataRepresentation* targetLayerRam) {
    LayerRAM* source = this;
    LayerRAM* target = dynamic_cast<LayerRAM*>(targetLayerRam);
    ivwAssert(target!=0, "Target representation missing.");

    //CPU image rescaling using image loader
    uvec2 targetDimensions  = target->getDimensions();
    /*void* rawData = ImageLoader::rescaleImageRAM(source, targetDimensions.x, targetDimensions.y);

    if (!rawData) return false;

    target->setData(rawData);*/

    return true;
}

void LayerRAM::setDimensions( uvec2 dimensions )
{
    resize(dimensions);
}

float LayerRAM::getDepthValue(const uvec2& pos) const{
    if (depthData_)
        return depthData_[posToIndex(pos, dimensions_)];
    else
        return 1.f;
}

void LayerRAM::allocateDepthData(){
    depthData_ = new float[dimensions_.x*dimensions_.y];
}

LayerRAM* createLayerRAM(const uvec2& dimension, const DataFormatBase* format) {
    switch (format->getId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("createLayerRAM", "Invalid format");
        return NULL;
    #define DataFormatIdMacro(i) case i: return new LayerRAMCustomPrecision<Data##i::type, Data##i::bits>(dimension); break;
    #include <inviwo/core/util/formatsdefinefunc.h>
    default:
        LogErrorCustom("createLayerRAM", "Invalid format or not implemented");
        return NULL;
    }
    return NULL;
}

} // namespace
