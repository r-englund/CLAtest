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
#include <modules/opencl/volume/volumeclglconverter.h>
#include <modules/opencl/syncclgl.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>
#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

VolumeRAM2CLGLConverter::VolumeRAM2CLGLConverter()
    : RepresentationConverterPackage<VolumeCLGL>()
{
    addConverter(new VolumeRAM2GLConverter());
    addConverter(new VolumeGL2CLGLConverter());
}

VolumeCLGL2RAMConverter::VolumeCLGL2RAMConverter()
    : RepresentationConverterType<VolumeRAM>()
{
}


DataRepresentation* VolumeCLGL2RAMConverter::createFrom(const DataRepresentation* source) {     
    DataRepresentation* destination = 0;
    const VolumeCLGL* volumeCLGL = static_cast<const VolumeCLGL*>(source);
    uvec3 dimension = volumeCLGL->getDimension();
    destination = createVolumeRAM(dimension, volumeCLGL->getDataFormat()); 
    const Texture3D* texture = volumeCLGL->getTexture();
    if (destination) {
        VolumeRAM* volumeRAM = static_cast<VolumeRAM*>(destination);
        texture->download(volumeRAM->getData());
        //const cl::CommandQueue& queue = OpenCL::instance()->getQueue();
        //queue.enqueueReadVolume(volumeCL->getVolume(), true, glm::svec3(0), glm::svec3(dimension, 1), 0, 0, volumeRAM->getData());
    } else {
        LogError("Invalid conversion or not implemented");
    }
    return destination;
}

void VolumeCLGL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const VolumeCLGL* volumeSrc = static_cast<const VolumeCLGL*>(source);
    VolumeRAM* volumeDst = static_cast<VolumeRAM*>(destination);
    if(volumeSrc->getDimension() != volumeDst->getDimension()) {
        volumeDst->setDimension(volumeSrc->getDimension());
    }
    volumeSrc->getTexture()->download(volumeDst->getData());

}


DataRepresentation* VolumeGL2CLGLConverter::createFrom(const DataRepresentation* source )
{
    DataRepresentation* destination = 0;
    const VolumeGL* volumeGL = static_cast<const VolumeGL*>(source);

    const Texture3D* data = volumeGL->getTexture();
    destination = new VolumeCLGL(volumeGL->getDimension(), volumeGL->getDataFormat(), data);
     
    return destination;
}

void VolumeGL2CLGLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
     // Do nothing since they are sharing data
    const VolumeGL* volumeSrc = static_cast<const VolumeGL*>(source);
    VolumeCLGL* volumeDst = static_cast<VolumeCLGL*>(destination);
    if(volumeSrc->getDimension() != volumeDst->getDimension()) {
        volumeDst->setDimension(volumeSrc->getDimension());
    }
}


DataRepresentation* VolumeCLGL2CLConverter::createFrom(const DataRepresentation* source )
{
    DataRepresentation* destination = 0;
    const VolumeCLGL* volumeCLGL = static_cast<const VolumeCLGL*>(source);
    uvec3 dimension = volumeCLGL->getDimension();;
    destination = new VolumeCL(dimension, volumeCLGL->getDataFormat());
    {SyncCLGL glSync;
    volumeCLGL->aquireGLObject(glSync.getGLSyncEvent());
    OpenCL::instance()->getQueue().enqueueCopyImage(volumeCLGL->getVolume(), static_cast<VolumeCL*>(destination)->getVolume(), glm::svec3(0), glm::svec3(0), glm::svec3(dimension));
    volumeCLGL->releaseGLObject(NULL, glSync.getLastReleaseGLEvent());
    }
    return destination;
}
void VolumeCLGL2CLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const VolumeCLGL* volumeSrc = static_cast<const VolumeCLGL*>(source);
    VolumeCL* volumeDst = static_cast<VolumeCL*>(destination);
    if(volumeSrc->getDimension() != volumeDst->getDimension()) {
        volumeDst->setDimension(volumeSrc->getDimension());
    }
    {SyncCLGL glSync;
    volumeSrc->aquireGLObject(glSync.getGLSyncEvent());
    OpenCL::instance()->getQueue().enqueueCopyImage(volumeSrc->getVolume(), volumeDst->getVolume(), glm::svec3(0), glm::svec3(0), glm::svec3(volumeSrc->getDimension()));
    volumeSrc->releaseGLObject(NULL, glSync.getLastReleaseGLEvent());
    }

}



VolumeCL2CLGLConverter::VolumeCL2CLGLConverter() : RepresentationConverterPackage<VolumeCLGL>()
{
    addConverter(new VolumeCL2RAMConverter());
    addConverter(new VolumeRAM2GLConverter());
    addConverter(new VolumeGL2CLGLConverter());
}

} // namespace
