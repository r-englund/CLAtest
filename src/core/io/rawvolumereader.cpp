#include "inviwo/core/io/rawvolumereader.h"


namespace inviwo {


RawVolumeReader::RawVolumeReader()
{}

void* RawVolumeReader::loadRawData(ReaderSettings& readerSettings) {

    if (readerSettings.dataFormat_ == "UINT8") {
        return  loadData<uint8_t>(readerSettings.rawFileAbsolutePath_, readerSettings.dimensions_ );
    }
    else if (readerSettings.dataFormat_ == "UINT16") {
        return  loadData<uint16_t>(readerSettings.rawFileAbsolutePath_, readerSettings.dimensions_ );
    }

    return 0;
}

} // namespace
