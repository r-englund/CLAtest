#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/util/typetostring.h>
#include <inviwo/core/io/rawvolumewriter.h>
#include <inviwo/core/io/datvolumewriter.h>
#include <inviwo/core/io/ivfvolumewriter.h>

namespace inviwo {

VolumeRAM::VolumeRAM(uvec3 dimensions, VolumeRepresentation::VolumeBorders border, DataFormatBase format)
    : VolumeRepresentation(dimensions, format, border), data_(0)
{}

VolumeRAM::~VolumeRAM() {
    deinitialize();
}

void VolumeRAM::initialize() {}

void VolumeRAM::deinitialize() {
    if(data_) {
        delete[] data_;
        data_ = NULL;
    }
}

void* VolumeRAM::getData() {
    return data_;
}

const void* VolumeRAM::getData() const {
    return const_cast<void*>(data_);
}

void VolumeRAM::saveData(std::string url) const {
    std::string fileExtension = UrlParser::getFileExtension(url);
    if (!fileExtension.empty()) {
        //TODO: better pattern for automatic data writer selection
        if (fileExtension=="dat") {
            WriterSettings writerSettings;
            writerSettings.rawFileAbsolutePath_ = UrlParser::replaceFileExtension(url, "raw");
            writerSettings.dimensions_ = dimensions_;
            writerSettings.dataFormat_ = getDataFormat().getString();
            writerSettings.texels_ = getData();
            DatVolumeWriter::writeDatFileSettings(url, writerSettings);
            RawVolumeWriter::saveRawData(writerSettings);
        }
        else if (fileExtension=="ivf") {
            IvfWriterSettings writerSettings;
            writerSettings.rawFileAbsolutePath_ = UrlParser::replaceFileExtension(url, "raw");
            writerSettings.dimensions_ = dimensions_;
            writerSettings.dataFormat_ = getDataFormat().getString();
            writerSettings.texels_ = getData();
            IvfVolumeWriter::writeIvfFileSettings(url, writerSettings);
            RawVolumeWriter::saveRawData(writerSettings);
        }
    }
}

VolumeRAM* createVolumeRAM(const uvec3& dimension, const DataFormatBase& format) {
    // TODO: Add more formats
    VolumeRAM* result = 0;
    switch (format.getId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("createVolumeRAM", "Invalid format");
    case FLOAT16:
        result = new VolumeRAMfloat16(dimension); break;
    case FLOAT32:
        result = new VolumeRAMfloat32(dimension); break;
    case FLOAT64:
        result = new VolumeRAMfloat64(dimension); break;
    case INT8:
        result = new VolumeRAMint8(dimension); break;
    case INT12:
        result = new VolumeRAMint12(dimension); break;
    case INT16:
        result = new VolumeRAMint16(dimension); break;
    case INT32:
        result = new VolumeRAMint32(dimension); break;
    case UINT8:
        result = new VolumeRAMuint8(dimension); break;
    case UINT12:
        result = new VolumeRAMuint12(dimension); break;
    case UINT16:
        result = new VolumeRAMuint16(dimension); break;
    case UINT32:
        result = new VolumeRAMuint32(dimension); break;
    default:
        LogErrorCustom("createVolumeRAM", "Invalid format or not implemented");
        break;
    }
    return result;
}

} // namespace

