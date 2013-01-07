#include "inviwo/core/datastructures/volumeramconverter.h"
#include "inviwo/core/datastructures/volumerepresentation.h"

namespace inviwo {

    VolumeRAMConverter::VolumeRAMConverter()
        : RepresentationConverterType<VolumeRAM>()
    {}

    VolumeRAMConverter::~VolumeRAMConverter() {}

    DataRepresentation* VolumeRAMConverter::convert(DataRepresentation* source) {
        DataRepresentation* destination=0;
        VolumeDisk* volumeDisk = 0;
        if (volumeDisk = dynamic_cast<VolumeDisk*>(source)) {
            if (volumeDisk->getDataFormat()=="UINT8") {
                void* data = volumeDisk->loadData<UINT8>();
                destination = new VolumeRAMuint8(static_cast<UINT8*>(data), volumeDisk->dimension());
            }
        }
        return destination;
    }

} // namespace
