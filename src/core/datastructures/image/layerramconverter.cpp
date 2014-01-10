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

#include <inviwo/core/datastructures/image/layerramconverter.h>
#include <inviwo/core/datastructures/image/layerramprecision.h>

namespace inviwo {

LayerDisk2RAMConverter::LayerDisk2RAMConverter()
    : RepresentationConverterType<LayerRAM>()
{}

LayerDisk2RAMConverter::~LayerDisk2RAMConverter() {}

/**
* Converts a LayerDisk representation to a RAM representation. This is done if a Image
* has a representation of LayerDisk and a LayerRAM representation is required. This is
* used in data.h.
*
* @param source is the input representation that is to be converted.
* @return the imageRAM representation of the file. Returns NULL if source is not a
* LayerDisk object.
**/
DataRepresentation* LayerDisk2RAMConverter::createFrom(const DataRepresentation* source) {
    const LayerDisk* layerDisk = static_cast<const LayerDisk*>(source);
    void* data = NULL;
    uvec2 dimension = layerDisk->getDimension();
    DataFormatId formatId = NOT_SPECIALIZED;
    
    if(dimension != uvec2(0))
        data = layerDisk->loadFileDataAndRescale(NULL, dimension, formatId);
    else
        data = layerDisk->loadFileData(NULL, dimension, formatId);

    switch (formatId) {
#define DataFormatIdMacro(i) case i: return new LayerRAM_##i(static_cast<Data##i::type*>(data), dimension, layerDisk->getLayerType());
#include <inviwo/core/util/formatsdefinefunc.h>
    default: 
        LogError("Cannot convert format from Disk to RAM:" << layerDisk->getDataFormat()->getString());
    }
        
    return NULL;
}

void LayerDisk2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const LayerDisk* layerSrc = static_cast<const LayerDisk*>(source);
    LayerRAM* layerDst = static_cast<LayerRAM*>(destination);
    DataFormatId formatId;
    layerSrc->loadFileDataAndRescale(layerDst->getData(), layerDst->getDimension(), formatId);
}

} // namespace
