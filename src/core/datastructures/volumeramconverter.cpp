#include "inviwo/core/datastructures/volumeramconverter.h"
#include "inviwo/core/datastructures/volumerepresentation.h"

namespace inviwo {

    VolumeDisk2RAMConverter::VolumeDisk2RAMConverter()
        : RepresentationConverterType<VolumeRAM>()
    {}

    VolumeDisk2RAMConverter::~VolumeDisk2RAMConverter() {}

    DataRepresentation* VolumeDisk2RAMConverter::convert(DataRepresentation* source) {
        DataRepresentation* destination=0;
        VolumeDisk* volumeDisk = 0;
        if (volumeDisk = dynamic_cast<VolumeDisk*>(source)) {
            if (volumeDisk->getDataFormat()=="UINT8") {
                void* data = volumeDisk->loadData<uint8_t>();
                destination = new VolumeRAMuint8(static_cast<uint8_t*>(data), volumeDisk->dimension());
            }
            else if (volumeDisk->getDataFormat()=="UINT16") {
                void* data = volumeDisk->loadData<uint16_t>();
                destination = new VolumeRAMuint16(static_cast<uint16_t*>(data), volumeDisk->dimension());
            }
        }
        return destination;
    }

} // namespace
