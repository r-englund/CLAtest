#include "inviwo/core/io/rawvolumereader.h"


namespace inviwo {


RawVolumeReader::RawVolumeReader()
{}

void* RawVolumeReader::loadRawData(ReaderSettings& readerSettings) {
    if (readerSettings.dataFormat_ == DataUINT8::str()) {
        return  loadData<DataUINT8::type>(readerSettings.rawFileAbsolutePath_, readerSettings.dimensions_ );
    }
    else if (readerSettings.dataFormat_ == DataUINT16::str()) {
        return  loadData<DataUINT16::type>(readerSettings.rawFileAbsolutePath_, readerSettings.dimensions_ );
    }

    return 0;
}

} // namespace
