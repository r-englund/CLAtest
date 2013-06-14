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

} // namespace
