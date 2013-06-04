#include <inviwo/core/datastructures/volume/volumedisk.h>
#include "inviwo/core/io/datvolumereader.h"

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
        }

    }

    void* VolumeDisk::loadRawData() {
        std::string fileExtension = UrlParser::getFileExtension(getSourceFile());
        if (!fileExtension.empty()) {
            //TODO: better pattern for automatic data reader selection
            if (fileExtension=="dat") {
                ReaderSettings readerSettings;
                DatVolumeReader::readDatFileSettings(getSourceFile(), readerSettings);
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
