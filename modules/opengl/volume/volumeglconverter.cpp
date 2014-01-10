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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include "volumeglconverter.h"
#include <inviwo/core/datastructures/volume/volumerepresentation.h>

namespace inviwo {

VolumeRAM2GLConverter::VolumeRAM2GLConverter()
    : RepresentationConverterType<VolumeGL>()
{}

VolumeRAM2GLConverter::~VolumeRAM2GLConverter() {}

DataRepresentation* VolumeRAM2GLConverter::createFrom(const DataRepresentation* source) {     
    const VolumeRAM* volumeRAM = static_cast<const VolumeRAM*>(source);
    return new VolumeGL(volumeRAM->getData(), volumeRAM->getDimension(), volumeRAM->getDataFormat());
}

void VolumeRAM2GLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const VolumeRAM* volumeSrc = static_cast<const VolumeRAM*>(source);
    VolumeGL* volumeDst = static_cast<VolumeGL*>(destination);
    if(volumeSrc->getDimension() != volumeDst->getDimension()) {
        volumeDst->setDimension(volumeSrc->getDimension());
    }
    volumeDst->upload(volumeSrc->getData());
}

VolumeGL2RAMConverter::VolumeGL2RAMConverter()
: RepresentationConverterType<VolumeRAM>()
{}

VolumeGL2RAMConverter::~VolumeGL2RAMConverter() {}

DataRepresentation* VolumeGL2RAMConverter::createFrom(const DataRepresentation* source) {
    const VolumeGL* volumeGL = static_cast<const VolumeGL*>(source);
    VolumeRAM* volume = createVolumeRAM(volumeGL->getDimension(), volumeGL->getDataFormat()); 
    if (volume) {
        volumeGL->getTexture()->download(volume->getData());
        return volume;
    } else {
        LogError("Cannot convert format from GL to RAM:" << volumeGL->getDataFormat()->getString());
    }
    return NULL;
}

void VolumeGL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const VolumeGL* volumeSrc = static_cast<const VolumeGL*>(source);
    VolumeRAM* volumeDst = static_cast<VolumeRAM*>(destination);
    if(volumeSrc->getDimension() != volumeDst->getDimension()) {
        volumeDst->setDimension(volumeSrc->getDimension());
    }
    volumeSrc->getTexture()->download(volumeDst->getData());
}

} // namespace
