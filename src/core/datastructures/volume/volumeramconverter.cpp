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
    void VolumeDisk2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
        const ImageDisk* imageSrc = dynamic_cast<const ImageDisk*>(source);
        ImageRAM* imageDst = dynamic_cast<ImageRAM*>(destination);
        if(imageSrc && imageDst) {
            // FIXME: The file loader should have a function that loads data into a preallocated location.
            imageDst->setData(imageSrc->loadFileData());
        }

    }

} // namespace
