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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#include "layerglconverter.h"

namespace inviwo {

LayerRAM2GLConverter::LayerRAM2GLConverter()
    : RepresentationConverterType<LayerGL>()
{}

LayerRAM2GLConverter::~LayerRAM2GLConverter() {}

DataRepresentation* LayerRAM2GLConverter::createFrom(const DataRepresentation* source) {
    const LayerRAM* layerRAM = static_cast<const LayerRAM*>(source);
    //This creates a texture from the defined input LayerRAM.
    if (layerRAM){       
        LayerGL* layerGL = new LayerGL(layerRAM->getDimension(), layerRAM->getLayerType(), layerRAM->getDataFormat());
        layerGL->getTexture()->initialize(layerRAM->getData());
        return layerGL;
    }
    return NULL;
}
void LayerRAM2GLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const LayerRAM* layerSrc = static_cast<const LayerRAM*>(source);
    LayerGL* layerDst = static_cast<LayerGL*>(destination);
    if(layerSrc->getDimension() != layerDst->getDimension()) {
        layerDst->resize(layerSrc->getDimension());
    }
    layerDst->getTexture()->upload(layerSrc->getData());
}

LayerGL2RAMConverter::LayerGL2RAMConverter()
    : RepresentationConverterType<LayerRAM>()
{}

LayerGL2RAMConverter::~LayerGL2RAMConverter() {}

DataRepresentation* LayerGL2RAMConverter::createFrom(const DataRepresentation* source) {
    const LayerGL* layerGL = static_cast<const LayerGL*>(source);
    LayerRAM* layerRAM = createLayerRAM(layerGL->getDimension(), layerGL->getLayerType(), layerGL->getDataFormat()); 
    if (layerRAM) {
        //layerGL->getTexture()->downloadToPBO();
        layerGL->getTexture()->download(layerRAM->getData());
        return layerRAM;
    } else {
        LogError("Cannot convert format from GL to RAM:" << layerGL->getDataFormat()->getString());
    }
    return NULL;
}

void LayerGL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const LayerGL* layerSrc = static_cast<const LayerGL*>(source);
    LayerRAM* layerDst = static_cast<LayerRAM*>(destination);
    if(layerSrc->getDimension() != layerDst->getDimension()) {
        layerDst->resize(layerSrc->getDimension());
    }
    //layerSrc->getTexture()->downloadToPBO();
    layerSrc->getTexture()->download(layerDst->getData());
}

} // namespace
