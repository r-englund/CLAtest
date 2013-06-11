#include "inviwo/core/io/datvolumewriter.h"

namespace inviwo {


DatVolumeWriter::DatVolumeWriter(const std::string filePath, WriterSettings writerSettings) 
    : VolumeWriter()
    , sourceFileAbsolutePath_(filePath)
    , writerFileSettings_(writerSettings)
{}

void DatVolumeWriter::writeData() {    
    writeRawVolumeData();
}    

void DatVolumeWriter::writeRawVolumeData() {

    //Check if raw data
    const void* rawData = writerFileSettings_.texels_;
    ivwAssert(rawData!=0, "raw data does not exist");

    //Write Dat file settings
    DatVolumeWriter::writeDatFileSettings(sourceFileAbsolutePath_, writerFileSettings_);    
    RawVolumeWriter::saveRawData(writerFileSettings_);

}

} // namespace
