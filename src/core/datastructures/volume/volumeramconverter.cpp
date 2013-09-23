#include <inviwo/core/datastructures/volume/volumeramconverter.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo {

VolumeDisk2RAMConverter::VolumeDisk2RAMConverter()
    : RepresentationConverterType<VolumeRAM>()
{}

VolumeDisk2RAMConverter::~VolumeDisk2RAMConverter() {}

DataRepresentation* VolumeDisk2RAMConverter::createFrom(const DataRepresentation* source) {
    const VolumeDisk* volumeDisk = dynamic_cast<const VolumeDisk*>(source);
    if (volumeDisk) {
        switch (volumeDisk->getDataFormatId()) {
        #define DataFormatIdMacro(i) case i: return new VolumeRAM_##i(static_cast<Data##i::type*>(const_cast<VolumeDisk*>(volumeDisk)->loadRawData()), volumeDisk->getDimensions());
        #include <inviwo/core/util/formatsdefinefunc.h>
        default: 
            LogError("Cannot convert format from Disk to RAM:" << volumeDisk->getDataFormat()->getString());
        }
    }
    return NULL;
}
void VolumeDisk2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const VolumeDisk* volumeSrc = dynamic_cast<const VolumeDisk*>( source );
    VolumeRAM* volumeDst = dynamic_cast<VolumeRAM*>(destination);
    if(volumeSrc && volumeDst) {
        // FIXME: The file loader should have a function that loads data into a preallocated location.
		volumeDst->setData(volumeSrc->loadRawData());
    }

}

} // namespace
