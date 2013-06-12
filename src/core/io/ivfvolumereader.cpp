#include "inviwo/core/io/ivfvolumereader.h"
#include "inviwo/core/datastructures/volume/volumeramprecision.h"
#include "inviwo/core/datastructures/volume/volumetypeclassification.h"

namespace inviwo {

IvfVolumeReader::IvfVolumeReader(const std::string filePath) 
    : VolumeReader()
    , sourceFileAbsolutePath_(filePath)
{}

Data* IvfVolumeReader::readData() {    
    IvfReaderSettings ivfReaderSettings;

    //Get reader settings
    IvfVolumeReader::readIvfFileSettings(sourceFileAbsolutePath_, ivfReaderSettings);

    //read raw data
    void* rawData = RawVolumeReader::loadRawData(ivfReaderSettings);
    ivwAssert(rawData!=0, "Unable to read raw file data");

    //Allocate volume handle
    Volume* data = new StandardVolume();
    DataRepresentation* dataRepresentation = 0;

    //Allocate representation
    if (ivfReaderSettings.dataFormat_ == "UINT8") {
        dataRepresentation = new VolumeRAMuint8(static_cast<uint8_t*>(rawData), ivfReaderSettings.dimensions_);            
    }
    else if (ivfReaderSettings.dataFormat_ == "UINT16") {
        dataRepresentation = new VolumeRAMuint16(static_cast<uint16_t*>(rawData), ivfReaderSettings.dimensions_);            
    }
    else {
        delete data;
        data = 0;
        return data;
    }

    //add representation to handle
    data->addRepresentation(dataRepresentation);
    return data;   
}    

} // namespace
