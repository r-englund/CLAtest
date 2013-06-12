#include <inviwo/core/datastructures/volume/volumedisk.h>
#include "inviwo/core/io/datvolumereader.h"
#include "inviwo/core/io/ivfvolumereader.h"

namespace inviwo {

    VolumeDisk::VolumeDisk(uvec3 dimensions, DataFormatBase format)
        : VolumeRepresentation(dimensions, format), DiskRepresentation()
    {}

    VolumeDisk::VolumeDisk(std::string srcFile, uvec3 dimensions, DataFormatBase format)
        : VolumeRepresentation(dimensions, format), DiskRepresentation(srcFile)
    {
        initialize();
    }

    VolumeDisk::~VolumeDisk() {}

    void VolumeDisk::initialize() {

        std::string fileExtension = UrlParser::getFileExtension(getSourceFile());
        if (!fileExtension.empty()) {
            //TODO: better pattern for automatic data reader selection
            if (fileExtension=="dat") {
                ReaderSettings readerSettings;
                DatVolumeReader::readDatFileSettings(getSourceFile(), readerSettings);
                //dataFormat_ = readerSettings.dataFormat_;
                dimensions_ = readerSettings.dimensions_;
            }
            else if (fileExtension=="ivf") {
                IvfReaderSettings readerSettings;
                IvfVolumeReader::readIvfFileSettings(getSourceFile(), readerSettings);
                dimensions_ = readerSettings.dimensions_;               
            }
        }

    }

    void* VolumeDisk::loadRawData() {
        std::string fileExtension = UrlParser::getFileExtension(getSourceFile());
        if (!fileExtension.empty()) {
            //TODO: better pattern for automatic data reader selection
            if (fileExtension=="dat") {
                ReaderSettings readerSettings;
                DatVolumeReader::readDatFileSettings(getSourceFile(), readerSettings);
                dimensions_ = readerSettings.dimensions_;
                return RawVolumeReader::loadRawData(readerSettings);
            }
            else if (fileExtension=="ivf") {
                IvfReaderSettings readerSettings;
                IvfVolumeReader::readIvfFileSettings(getSourceFile(), readerSettings);
                dimensions_ = readerSettings.dimensions_;
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
