#include <inviwo/core/io/ivfvolumereader.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>

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
    if (ivfReaderSettings.dataFormat_ == "") {
        delete data;
        data = 0;
        return data;    
    }
    #define DataFormatIdMacro(i) else if (ivfReaderSettings.dataFormat_ == "##i") { dataRepresentation = new VolumeRAM_##i(static_cast<Data##i::type*>(rawData), ivfReaderSettings.dimensions_); }
    #include <inviwo/core/util/formatsdefinefunc.h>
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
