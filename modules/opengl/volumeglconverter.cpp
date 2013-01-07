#include "volumeglconverter.h"
#include "inviwo/core/datastructures/volumerepresentation.h"

namespace inviwo {

    VolumeGLConverter::VolumeGLConverter()
        : RepresentationConverterType<VolumeGL>()
    {}

    VolumeGLConverter::~VolumeGLConverter() {}

    DataRepresentation* VolumeGLConverter::convert(DataRepresentation* source) {     
        DataRepresentation* destination = 0;
        std::string dataFormat("");
        ivec3 dimension(0,0,0);
        VolumeRAM* volumeRAM = 0;
        VolumeDisk* volumeDisk = 0;
        void* data = 0;

        if (volumeRAM = dynamic_cast<VolumeRAM*>(source)) {
            if (dynamic_cast<VolumeRAMuint8*>(volumeRAM)) {
                dimension = volumeRAM->dimension();
                dataFormat = volumeRAM->getDataFormat();
                data = volumeRAM->getData();
            }
        }
        else if (volumeDisk = dynamic_cast<VolumeDisk*>(source)) {            
            dimension = volumeDisk->dimension();
            dataFormat = volumeDisk->getDataFormat();
            //TODO: Remove this function call
            data = volumeDisk->loadData<GLubyte>();
        }

        if (dataFormat == "UINT8") {
            destination = new VolumeGLuint8(static_cast<UINT8*>(data), dimension);
        }

        return destination;
    }

} // namespace
