#include <inviwo/core/datastructures/volumeramconverter.h>
#include <inviwo/core/datastructures/volumeramprecision.h>

namespace inviwo {

    VolumeDisk2RAMConverter::VolumeDisk2RAMConverter()
        : RepresentationConverterType<VolumeRAM>()
    {}

    VolumeDisk2RAMConverter::~VolumeDisk2RAMConverter() {}

    DataRepresentation* VolumeDisk2RAMConverter::convert(DataRepresentation* source) {
        VolumeDisk* volumeDisk = dynamic_cast<VolumeDisk*>(source);
        if (volumeDisk) {
            switch (volumeDisk->getDataFormatId()) {
            case UINT8:
                return new VolumeRAMuint8(static_cast<DataUINT8::type*>(volumeDisk->loadRawData()), volumeDisk->getDimensions());
            case UINT16:
                return new VolumeRAMuint16(static_cast<DataUINT16::type*>(volumeDisk->loadRawData()), volumeDisk->getDimensions());
            }
        }
        return NULL;
    }

} // namespace
