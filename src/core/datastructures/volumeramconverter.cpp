#include <inviwo/core/datastructures/volumeramconverter.h>
#include <inviwo/core/datastructures/volumerepresentation.h>

namespace inviwo {

    VolumeDisk2RAMConverter::VolumeDisk2RAMConverter()
        : RepresentationConverterType<VolumeRAM>()
    {}

    VolumeDisk2RAMConverter::~VolumeDisk2RAMConverter() {}

    DataRepresentation* VolumeDisk2RAMConverter::convert(DataRepresentation* source) {
        DataRepresentation* destination=0;
        VolumeDisk* volumeDisk = dynamic_cast<VolumeDisk*>(source);
        if (volumeDisk) {
            if (volumeDisk->getDataFormat()=="UINT8") {
                void* data = volumeDisk->loadRawData();    
                destination = new VolumeRAMuint8(static_cast<uint8_t*>(data), volumeDisk->getDimension());
            }
            else if (volumeDisk->getDataFormat()=="UINT16") {
               void* data = volumeDisk->loadRawData();
                destination = new VolumeRAMuint16(static_cast<uint16_t*>(data), volumeDisk->getDimension());
            }
        }
        return destination;
    }

} // namespace
