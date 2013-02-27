#include <inviwo/core/datastructures/volumedisk.h>
#include "inviwo/core/io/datvolumereader.h"

namespace inviwo {

    VolumeDisk::VolumeDisk(std::string srcFile)
        : VolumeRepresentation(uvec3(128,128,128), DataUINT8())
    {
        sourceFile_ = srcFile;
        initialize();
    }

    VolumeDisk::~VolumeDisk() {}

    void VolumeDisk::initialize() {

        std::string fileExtension = UrlParser::getFileExtension(sourceFile_);
        if (!fileExtension.empty()) {
            //TODO: better pattern for automatic data reader selection
            if (fileExtension=="dat") {
                ReaderSettings readerSettings;
                DatVolumeReader::readDatFileSettings(sourceFile_, readerSettings);
                //dataFormat_ = readerSettings.dataFormat_;
                dimensions_ = readerSettings.dimensions_;
            }
        }

    }

    void* VolumeDisk::loadRawData() {
        std::string fileExtension = UrlParser::getFileExtension(sourceFile_);
        if (!fileExtension.empty()) {
            //TODO: better pattern for automatic data reader selection
            if (fileExtension=="dat") {
                ReaderSettings readerSettings;
                DatVolumeReader::readDatFileSettings(sourceFile_, readerSettings);
                return RawVolumeReader::loadRawData(readerSettings);                
            }
        }

        return 0;
    }

    void VolumeDisk::deinitialize() {
    }

    DataRepresentation* VolumeDisk::clone() {
        return new VolumeDisk(sourceFile_);
    }



} // namespace
