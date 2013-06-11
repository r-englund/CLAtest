#include "inviwo/core/io/rawvolumewriter.h"


namespace inviwo {


RawVolumeWriter::RawVolumeWriter()
{}

void RawVolumeWriter::saveRawData(WriterSettings& writerSettings) {
    if (writerSettings.dataFormat_ == DataUINT8::str()) {
        saveData<DataUINT8::type>(writerSettings.rawFileAbsolutePath_, writerSettings.dimensions_, writerSettings.texels_);
    }
    else if (writerSettings.dataFormat_ == DataUINT16::str()) {
        saveData<DataUINT16::type>(writerSettings.rawFileAbsolutePath_, writerSettings.dimensions_, writerSettings.texels_ );
    }
}

} // namespace
