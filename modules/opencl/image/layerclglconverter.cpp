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
#include <modules/opencl/image/layerclglconverter.h>
#include <modules/opencl/syncclgl.h>
#include <inviwo/core/datastructures/image/layerrepresentation.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

LayerRAM2CLGLConverter::LayerRAM2CLGLConverter()
    : RepresentationConverterPackage<LayerCLGL>() {
    addConverter(new LayerRAM2GLConverter());
    addConverter(new LayerGL2CLGLConverter());
}

LayerCLGL2RAMConverter::LayerCLGL2RAMConverter()
    : RepresentationConverterType<LayerRAM>() {
}


DataRepresentation* LayerCLGL2RAMConverter::createFrom(const DataRepresentation* source) {     
    DataRepresentation* destination = 0;
    const LayerCLGL* layerCLGL = static_cast<const LayerCLGL*>(source);
    uvec2 dimension = layerCLGL->getDimension();
    destination = createLayerRAM(dimension, layerCLGL->getLayerType(), layerCLGL->getDataFormat()); 
    const Texture2D* texture = layerCLGL->getTexture();
    if (destination) {
        LayerRAM* layerRAM = static_cast<LayerRAM*>(destination);
        texture->download(layerRAM->getData());
        //const cl::CommandQueue& queue = OpenCL::instance()->getQueue();
        //queue.enqueueReadLayer(layerCL->getLayer(), true, glm::svec3(0), glm::svec3(dimension, 1), 0, 0, layerRAM->getData());
    } else {
        LogError("Invalid conversion or not implemented");
    }
    return destination;
}

void LayerCLGL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const LayerCLGL* layerSrc = static_cast<const LayerCLGL*>(source);
    LayerRAM* layerDst = static_cast<LayerRAM*>(destination);
    if(layerSrc->getDimension() != layerDst->getDimension()) {
        layerDst->setDimension(layerSrc->getDimension());
    }
    layerSrc->getTexture()->download(layerDst->getData());
}

LayerCLGL2GLConverter::LayerCLGL2GLConverter(): RepresentationConverterType<LayerGL>() {

}

DataRepresentation* LayerCLGL2GLConverter::createFrom( const DataRepresentation* source ) {
    DataRepresentation* destination = 0;
    const LayerCLGL* src = static_cast<const LayerCLGL*>(source);
    // TODO: Do we need to check if the LayerCLGL texture is valid to use? 
    // It should not have been deleted since no LayerGL representation existed.
    Texture2D* tex = const_cast<Texture2D*>(src->getTexture());
    destination = new LayerGL(src->getDimension(), src->getLayerType(), src->getDataFormat(), const_cast<Texture2D*>(src->getTexture()));
    // Increase reference count to indicate that LayerGL is also using the texture
    tex->increaseRefCount();
    return destination;
}

void LayerCLGL2GLConverter::update( const DataRepresentation* source, DataRepresentation* destination ) {
    // Do nothing since they share data
}

DataRepresentation* LayerCLGL2CLConverter::createFrom(const DataRepresentation* source) {
#ifdef _DEBUG
    LogWarn("Performance warning: Use shared CLGL representation instead of CL ");
#endif
    DataRepresentation* destination = 0;
    const LayerCLGL* src = static_cast<const LayerCLGL*>(source);
    destination = new LayerCL(src->getDimension(), src->getLayerType(), src->getDataFormat());
    {SyncCLGL glSync;
    src->aquireGLObject(glSync.getGLSyncEvent());
    OpenCL::instance()->getQueue().enqueueCopyImage(src->get(), static_cast<LayerCL*>(destination)->get(), glm::svec3(0), glm::svec3(0), glm::svec3(src->getDimension(), 1));
    src->releaseGLObject(NULL, glSync.getLastReleaseGLEvent());
    }
    return destination;
}

void LayerCLGL2CLConverter::update( const DataRepresentation* source, DataRepresentation* destination ) {
    const LayerCLGL* src = static_cast<const LayerCLGL*>(source);
    LayerCL* dst = static_cast<LayerCL*>(destination);
    if(src->getDimension() != dst->getDimension()) {
        dst->setDimension(src->getDimension());
    }
    {SyncCLGL glSync;
    src->aquireGLObject(glSync.getGLSyncEvent());
    OpenCL::instance()->getQueue().enqueueCopyImage(src->get(), dst->get(), glm::svec3(0), glm::svec3(0), glm::svec3(src->getDimension(), 1));
    src->releaseGLObject(NULL, glSync.getLastReleaseGLEvent());
    }
}


DataRepresentation* LayerGL2CLGLConverter::createFrom(const DataRepresentation* source ) {
    DataRepresentation* destination = 0;
    const LayerGL* layerGL = static_cast<const LayerGL*>(source);
    destination = new LayerCLGL(layerGL->getDimension(), layerGL->getLayerType(), layerGL->getDataFormat(), const_cast<Texture2D*>(layerGL->getTexture()));
    return destination;
}

void LayerGL2CLGLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const LayerGL* layerSrc = static_cast<const LayerGL*>(source);
    LayerCLGL* layerDst = static_cast<LayerCLGL*>(destination);
    if(layerSrc->getDimension() != layerDst->getDimension()) {
        layerDst->setDimension(layerSrc->getDimension());
    }
}

LayerCL2CLGLConverter::LayerCL2CLGLConverter() : RepresentationConverterPackage<LayerCLGL>() {
    addConverter(new LayerCL2RAMConverter());
    addConverter(new LayerRAM2GLConverter());
    addConverter(new LayerGL2CLGLConverter());
}




} // namespace
