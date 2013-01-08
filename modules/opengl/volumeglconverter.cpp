#include "volumeglconverter.h"
#include "inviwo/core/datastructures/volumerepresentation.h"

namespace inviwo {

    VolumeRAM2GLConverter::VolumeRAM2GLConverter()
        : RepresentationConverterType<VolumeGL>()
    {}

    VolumeRAM2GLConverter::~VolumeRAM2GLConverter() {}

    DataRepresentation* VolumeRAM2GLConverter::convert(DataRepresentation* source) {     
        DataRepresentation* destination = 0;
        std::string dataFormat("");
        ivec3 dimension(0,0,0);
        void* data = 0;
        VolumeRAM* volumeRAM = dynamic_cast<VolumeRAM*>(source);
        if (volumeRAM) {
            if (dynamic_cast<VolumeRAMuint8*>(volumeRAM)) {
                dimension = volumeRAM->dimension();
                dataFormat = volumeRAM->getDataFormat();
                data = volumeRAM->getData();
            }
            else if (dynamic_cast<VolumeRAMuint16*>(volumeRAM)) {
                dimension = volumeRAM->dimension();
                dataFormat = volumeRAM->getDataFormat();
                data = volumeRAM->getData();
            }
        }        


        if (dataFormat == "UINT8") {
            destination = new VolumeGLuint8(static_cast<uint8_t*>(data), dimension);
        }
        else if (dataFormat == "UINT16") {
            destination = new VolumeGLuint16(static_cast<uint16_t*>(data), dimension);
        }

        return destination;
    }

} // namespace
