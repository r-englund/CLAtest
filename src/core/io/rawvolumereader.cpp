#include "inviwo/core/io/rawvolumereader.h"


namespace inviwo {

RawVolumeReader::ReaderSettings::ReaderSettings(std::string rawFile, ivec3 dimensions, std::string format) 
    : rawFileAbsolutePath_(rawFile)
    , dimensions_(dimensions)
    , dataFormat_(format)
{}

RawVolumeReader::RawVolumeReader()
{}

void* RawVolumeReader::loadRawData(RawVolumeReader::ReaderSettings& readerSettings) {

    if (readerSettings.dataFormat_ == "UINT8") {
        return  loadData<uint8_t>(readerSettings.rawFileAbsolutePath_, readerSettings.dimensions_ );
    }
    else if (readerSettings.dataFormat_ == "UINT16") {
        return  loadData<uint16_t>(readerSettings.rawFileAbsolutePath_, readerSettings.dimensions_ );
    }

    return 0;
}

} // namespace
