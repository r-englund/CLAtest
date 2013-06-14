#include <inviwo/core/datastructures/volume/volumeramconverter.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>

namespace inviwo {

    VolumeDisk2RAMConverter::VolumeDisk2RAMConverter()
        : RepresentationConverterType<VolumeRAM>()
    {}

    VolumeDisk2RAMConverter::~VolumeDisk2RAMConverter() {}

    DataRepresentation* VolumeDisk2RAMConverter::createFrom(const DataRepresentation* source) {
        const VolumeDisk* volumeDisk = dynamic_cast<const VolumeDisk*>(source);
        if (volumeDisk) {
            switch (volumeDisk->getDataFormatId()) {
            case UINT8:
                return new VolumeRAMuint8(static_cast<DataUINT8::type*>(const_cast<VolumeDisk*>(volumeDisk)->loadRawData()), volumeDisk->getDimensions());
            case UINT16:
                return new VolumeRAMuint16(static_cast<DataUINT16::type*>(const_cast<VolumeDisk*>(volumeDisk)->loadRawData()), volumeDisk->getDimensions());
            }
        }
        return NULL;
    }

} // namespace
