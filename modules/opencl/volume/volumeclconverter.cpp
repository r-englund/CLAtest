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

#include <modules/opencl/volume/volumeclconverter.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

VolumeRAM2CLConverter::VolumeRAM2CLConverter()
    : RepresentationConverterType<VolumeCL>()
{}

DataRepresentation* VolumeRAM2CLConverter::createFrom(const DataRepresentation* source) {     
    DataRepresentation* destination = 0;
    const VolumeRAM* volumeRAM = static_cast<const VolumeRAM*>(source);
    uvec3 dimension = volumeRAM->getDimension();
    const void* data = volumeRAM->getData();
    destination = new VolumeCL(dimension, volumeRAM->getDataFormat(), data);

    return destination;
}

void VolumeRAM2CLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const VolumeRAM* volumeSrc = static_cast<const VolumeRAM*>(source);
    VolumeCL* volumeDst = static_cast<VolumeCL*>(destination);
    if(volumeSrc->getDimension() != volumeDst->getDimension()) {
        volumeDst->setDimension(volumeSrc->getDimension());
    }
    volumeDst->upload(volumeSrc->getData());
}

VolumeCL2RAMConverter::VolumeCL2RAMConverter()
    : RepresentationConverterType<VolumeRAM>()
{}


DataRepresentation* VolumeCL2RAMConverter::createFrom(const DataRepresentation* source) {     
    DataRepresentation* destination = 0;
    const VolumeCL* volumeCL = static_cast<const VolumeCL*>(source);
    uvec3 dimension = volumeCL->getDimension();
    destination = createVolumeRAM(dimension, volumeCL->getDataFormat());

    if (destination) {
        VolumeRAM* volumeRAM = static_cast<VolumeRAM*>(destination);
        volumeCL->download(volumeRAM->getData());
        //const cl::CommandQueue& queue = OpenCL::getInstance()->getQueue();
        //queue.enqueueReadImage(volumeCL->getVolume(), true, glm::svec3(0), glm::svec3(dimension), 0, 0, volumeRAM->getData());
    }      
    return destination;
}

void VolumeCL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const VolumeCL* volumeSrc = static_cast<const VolumeCL*>(source);
    VolumeRAM* volumeDst = static_cast<VolumeRAM*>(destination);
    if(volumeSrc->getDimension() != volumeDst->getDimension()) {
        volumeDst->setDimension(volumeSrc->getDimension());
    }
    volumeSrc->download(volumeDst->getData());
    if(volumeDst->hasNormalizedHistogram())
        volumeDst->getNormalizedHistogram()->setValid(false);
}

} // namespace
