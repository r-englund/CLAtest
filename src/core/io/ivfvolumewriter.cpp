#include <inviwo/core/io/ivfvolumewriter.h>
#include <inviwo/core/io/rawvolumewriter.h>

namespace inviwo {


IvfVolumeWriter::IvfVolumeWriter(const std::string filePath, IvfWriterSettings writerSettings) 
    : VolumeWriter()
    , sourceFileAbsolutePath_(filePath)
    , writerFileSettings_(writerSettings)
{}

void IvfVolumeWriter::writeData() {    
    writeRawVolumeData();
}    

void IvfVolumeWriter::writeRawVolumeData() {
    //Check if raw data
    const void* rawData = writerFileSettings_.texels_;
    ivwAssert(rawData!=0, "raw data does not exist");

    //Write Dat file settings
    IvfVolumeWriter::writeIvfFileSettings(sourceFileAbsolutePath_, writerFileSettings_);    
    RawVolumeWriter::saveRawData(writerFileSettings_);
}

} // namespace
