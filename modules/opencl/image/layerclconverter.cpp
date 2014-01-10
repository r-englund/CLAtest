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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#include <modules/opencl/image/layerclconverter.h>
#include <inviwo/core/datastructures/image/layerrepresentation.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

LayerRAM2CLConverter::LayerRAM2CLConverter()
    : RepresentationConverterType<LayerCL>()
{}

DataRepresentation* LayerRAM2CLConverter::createFrom(const DataRepresentation* source) {     
    DataRepresentation* destination = 0;
    const LayerRAM* layerRAM = static_cast<const LayerRAM*>(source);
    uvec2 dimension = layerRAM->getDimension();;
    const void* data = layerRAM->getData();
    destination = new LayerCL(dimension, layerRAM->getLayerType(), layerRAM->getDataFormat(), data);        
   
    return destination;
}
void LayerRAM2CLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const LayerRAM* layerSrc = static_cast<const LayerRAM*>(source);
    LayerCL* layerDst = static_cast<LayerCL*>(destination);
    if(layerSrc->getDimension() != layerDst->getDimension()) {
        layerDst->resize(layerSrc->getDimension());
    }
    layerDst->upload(layerSrc->getData());
}

LayerCL2RAMConverter::LayerCL2RAMConverter()
    : RepresentationConverterType<LayerRAM>()
{}


DataRepresentation* LayerCL2RAMConverter::createFrom(const DataRepresentation* source) {     
    DataRepresentation* destination = 0;
    const LayerCL* layerCL = static_cast<const LayerCL*>(source);
    uvec2 dimension = layerCL->getDimension();
    destination = createLayerRAM(dimension, layerCL->getLayerType(), layerCL->getDataFormat()); 
    
    if (destination) {
        LayerRAM* layerRAM = static_cast<LayerRAM*>(destination);
        layerCL->download(layerRAM->getData());
        //const cl::CommandQueue& queue = OpenCL::getInstance()->getQueue();
        //queue.enqueueReadLayer(layerCL->getLayer(), true, glm::svec3(0), glm::svec3(dimension, 1), 0, 0, layerRAM->getData());
    } else {
        LogError("Invalid conversion or not implemented");
    }
    return destination;
}

void LayerCL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const LayerCL* layerSrc = static_cast<const LayerCL*>(source);
    LayerRAM* layerDst = static_cast<LayerRAM*>(destination);
    if(layerSrc->getDimension() != layerDst->getDimension()) {
        layerDst->resize(layerSrc->getDimension());
    }
    layerSrc->download(layerDst->getData());

}

} // namespace
