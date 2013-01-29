#include "inviwo/core/io/datvolumereader.h"
#include "inviwo/core/datastructures/volumeram.h"
#include "inviwo/core/datastructures/volumetypeclassification.h"

namespace inviwo {


DatVolumeReader::DatVolumeReader(const std::string filePath) 
    : VolumeReader()
    , sourceFileAbsolutePath_(filePath)
{}

Data* DatVolumeReader::readData() {    
    return readRawVolumeData();
}    

Data* DatVolumeReader::readRawVolumeData() {

    ReaderSettings readerSettings;

    //Get reader settings
    DatVolumeReader::readDatFileSettings(sourceFileAbsolutePath_, readerSettings);    

    //read raw data
    void* rawData = RawVolumeReader::loadRawData(readerSettings);

    ivwAssert(rawData!=0, "Unable to read raw file data");        

    //Allocate volume handle
    Volume* data = new StandardVolume();
    DataRepresentation* dataRepresentation = 0;

    //Allocate representation
    if (readerSettings.dataFormat_ == "UINT8") {
        dataRepresentation = new VolumeRAMuint8(static_cast<uint8_t*>(rawData), readerSettings.dimensions_);            
    }
    else if (readerSettings.dataFormat_ == "UINT16") {
        dataRepresentation = new VolumeRAMuint16(static_cast<uint16_t*>(rawData), readerSettings.dimensions_);            
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
