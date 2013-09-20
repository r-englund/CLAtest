#include <inviwo/core/datastructures/volume/volumedisk.h>
#include "inviwo/core/io/datvolumereader.h"
#include "inviwo/core/io/ivfvolumereader.h"

namespace inviwo {

    VolumeDisk::VolumeDisk(uvec3 dimensions, const DataFormatBase* format)
        : VolumeRepresentation(dimensions, format), DiskRepresentation()
    {}

    VolumeDisk::VolumeDisk(std::string srcFile, uvec3 dimensions, const DataFormatBase* format)
        : VolumeRepresentation(dimensions, format), DiskRepresentation(srcFile)
    {
        initialize();
    }

    VolumeDisk::~VolumeDisk() {}

    void VolumeDisk::initialize() {

        std::string fileExtension = URLParser::getFileExtension(getSourceFile());
        if (!fileExtension.empty()) {
            //TODO: better pattern for automatic data reader selection
            ReaderSettings* readerSettings = 0;
            if (fileExtension=="dat") {
                ReaderSettings* datReaderSettings = new ReaderSettings();
                DatVolumeReader::readDatFileSettings(getSourceFile(), *datReaderSettings);
                readerSettings = datReaderSettings;
            }
            else if (fileExtension=="ivf") {
                IvfReaderSettings* ivfReaderSettings = new IvfReaderSettings();                
                IvfVolumeReader::readIvfFileSettings(getSourceFile(), *ivfReaderSettings); 
                readerSettings = ivfReaderSettings;
            }

            if (readerSettings) {
                dimensions_ = readerSettings->dimensions_;
                if (readerSettings->dataFormat_== DataUINT8::str()) {
                    setDataFormat(DataUINT8::get());
                }
                else if(readerSettings->dataFormat_== DataUINT16::str()) {
                    setDataFormat(DataUINT8::get());
                }
                delete readerSettings;
            }
        }

    }

    void* VolumeDisk::loadRawData() const {
        std::string fileExtension = URLParser::getFileExtension(getSourceFile());
        if (!fileExtension.empty()) {
            //TODO: better pattern for automatic data reader selection
            if (fileExtension=="dat") {
                ReaderSettings readerSettings;
                DatVolumeReader::readDatFileSettings(getSourceFile(), readerSettings);
                ivwAssert(dimensions_==readerSettings.dimensions_, "Dimension is invalid.");
                return RawVolumeReader::loadRawData(readerSettings);
            }
            else if (fileExtension=="ivf") {
                IvfReaderSettings readerSettings;
                IvfVolumeReader::readIvfFileSettings(getSourceFile(), readerSettings);
                ivwAssert(dimensions_==readerSettings.dimensions_, "Dimension is invalid.");
                return RawVolumeReader::loadRawData(readerSettings);                
            }
        }

        return 0;
    }

    void VolumeDisk::deinitialize() {
    }

    DataRepresentation* VolumeDisk::clone() const {
        return new VolumeDisk(getSourceFile());
    }



} // namespace
