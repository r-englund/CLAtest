#include "volumeglconverter.h"
#include <inviwo/core/datastructures/volume/volumerepresentation.h>

namespace inviwo {

VolumeRAM2GLConverter::VolumeRAM2GLConverter()
    : RepresentationConverterType<VolumeGL>()
{}

VolumeRAM2GLConverter::~VolumeRAM2GLConverter() {}

DataRepresentation* VolumeRAM2GLConverter::createFrom(const DataRepresentation* source) {     
    const VolumeRAM* volumeRAM = dynamic_cast<const VolumeRAM*>(source);
    if (volumeRAM) {
        return new VolumeGL(volumeRAM->getData(), volumeRAM->getDimensions(), volumeRAM->getDataFormat());
    }
    return NULL;
}

void VolumeRAM2GLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const VolumeRAM* volumeSrc = dynamic_cast<const VolumeRAM*>(source);
    VolumeGL* volumeDst = dynamic_cast<VolumeGL*>(destination);
    if(volumeSrc && volumeDst) {
        volumeDst->upload(volumeSrc->getData());
    }
}

VolumeGL2RAMConverter::VolumeGL2RAMConverter()
: RepresentationConverterType<VolumeRAM>()
{}

VolumeGL2RAMConverter::~VolumeGL2RAMConverter() {}

DataRepresentation* VolumeGL2RAMConverter::createFrom(const DataRepresentation* source) {
    const VolumeGL* volumeGL = dynamic_cast<const VolumeGL*>(source);
    if(volumeGL){
        VolumeRAM* volume = createVolumeRAM(volumeGL->getDimensions(), volumeGL->getDataFormat()); 
        if (volume) {
            volumeGL->getTexture()->download(volume->getData());
            return volume;
        } else {
            LogError("Cannot convert format from GL to RAM:" << volumeGL->getDataFormat()->getString());
        }
    }
    else {
        LogError("Source could not be cast to VolumeGL");
    }
    return NULL;
}

void VolumeGL2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const VolumeGL* volumeSrc = dynamic_cast<const VolumeGL*>(source);
    VolumeRAM* volumeDst = dynamic_cast<VolumeRAM*>(destination);
    if(volumeSrc && volumeDst) {
        // FIXME: OpenGL color should not have both depth and color
        if (volumeDst->getDimensions()==volumeSrc->getDimensions(),"GL and Ram representations are expected to have same dimensions.")
            volumeSrc->getTexture()->download(volumeDst->getData());
    }
}

} // namespace
