#include "volumeglconverter.h"
#include <inviwo/core/datastructures/volume/volumerepresentation.h>

namespace inviwo {

VolumeRAM2GLConverter::VolumeRAM2GLConverter()
    : RepresentationConverterType<VolumeGL>()
{}

VolumeRAM2GLConverter::~VolumeRAM2GLConverter() {}

DataRepresentation* VolumeRAM2GLConverter::createFrom(const DataRepresentation* source) {     
    const VolumeRAM* volumeRAM = static_cast<const VolumeRAM*>(source);
    return new VolumeGL(volumeRAM->getData(), volumeRAM->getDimensions(), volumeRAM->getDataFormat());
}

void VolumeRAM2GLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const VolumeRAM* volumeSrc = static_cast<const VolumeRAM*>(source);
    VolumeGL* volumeDst = static_cast<VolumeGL*>(destination);
    if(volumeSrc->getDimensions() != volumeDst->getDimensions()) {
        volumeDst->setDimensions(volumeSrc->getDimensions());
    }
    volumeDst->upload(volumeSrc->getData());
}

VolumeGL2RAMConverter::VolumeGL2RAMConverter()
: RepresentationConverterType<VolumeRAM>()
{}

VolumeGL2RAMConverter::~VolumeGL2RAMConverter() {}

DataRepresentation* VolumeGL2RAMConverter::createFrom(const DataRepresentation* source) {
    const VolumeGL* volumeGL = static_cast<const VolumeGL*>(source);
    VolumeRAM* volume = createVolumeRAM(volumeGL->getDimensions(), volumeGL->getDataFormat()); 
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
    if(volumeSrc->getDimensions() != volumeDst->getDimensions()) {
        volumeDst->setDimensions(volumeSrc->getDimensions());
    }
    volumeSrc->getTexture()->download(volumeDst->getData());
}

} // namespace
