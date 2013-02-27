#include "volumeglconverter.h"
#include <inviwo/core/datastructures/volumerepresentation.h>

namespace inviwo {

VolumeRAM2GLConverter::VolumeRAM2GLConverter()
    : RepresentationConverterType<VolumeGL>()
{}

VolumeRAM2GLConverter::~VolumeRAM2GLConverter() {}

DataRepresentation* VolumeRAM2GLConverter::convert(DataRepresentation* source) {     
    VolumeRAM* volumeRAM = dynamic_cast<VolumeRAM*>(source);
    if (volumeRAM) {
        switch (volumeRAM->getDataFormatId()) {
            case UINT8:
              return new VolumeGLuint8(static_cast<DataUINT8::type*>(volumeRAM->getData()), volumeRAM->getDimensions());
            case UINT16:
              return new VolumeGLuint16(static_cast<DataUINT16::type*>(volumeRAM->getData()), volumeRAM->getDimensions());
        }
    }
    return NULL;
}

} // namespace
