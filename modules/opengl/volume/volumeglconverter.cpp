#include "volumeglconverter.h"
#include <inviwo/core/datastructures/volume/volumerepresentation.h>

namespace inviwo {

VolumeRAM2GLConverter::VolumeRAM2GLConverter()
    : RepresentationConverterType<VolumeGL>()
{}

VolumeRAM2GLConverter::~VolumeRAM2GLConverter() {}

DataRepresentation* VolumeRAM2GLConverter::convert(DataRepresentation* source) {     
    VolumeRAM* volumeRAM = dynamic_cast<VolumeRAM*>(source);
    if (volumeRAM) {
        return new VolumeGL(volumeRAM->getData(), volumeRAM->getDimensions(), volumeRAM->getDataFormat());
    }
    return NULL;
}

} // namespace
