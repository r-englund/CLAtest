#include <inviwo/core/io/rawvolumereader.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>
#include <inviwo/core/util/urlparser.h>
#include <inviwo/core/util/formatconversion.h>

namespace inviwo {

RawVolumeReader::RawVolumeReader()
    : DataReaderType<Volume>()
    , rawFile_("")
    , littleEndian_(true)
    , dimension_(uvec3(0, 0, 0))
    , format_(NULL) {
        addExtension(FileExtension("raw", "Raw binary file"));
    }

RawVolumeReader::RawVolumeReader(const RawVolumeReader& rhs)
    : DataReaderType<Volume>(rhs)
    , rawFile_(rhs.rawFile_)
    , littleEndian_(true)
    , dimension_(rhs.dimension_)
    , format_(rhs.format_) {};

RawVolumeReader& RawVolumeReader::operator=(const RawVolumeReader& that) {
    if(this != &that) {
        rawFile_ = that.rawFile_;
        littleEndian_ = that.littleEndian_;
        dimension_ = that.dimension_;
        format_ = that.format_;
        DataReaderType<Volume>::operator=(that);
    }
    return *this;
}

RawVolumeReader* RawVolumeReader::clone() const {
    return new RawVolumeReader(*this);
}


Volume* RawVolumeReader::readMetaData(std::string filePath) {

    if(!URLParser::fileExists(filePath)) {
        filePath = URLParser::addBasePath(filePath);
        if(!URLParser::fileExists(filePath)) {
            throw DataReaderException("Error: Input file: " + filePath + " does not exist");
        }
    }

    std::string fileDirectory = URLParser::getFileDirectory(filePath);
    std::string fileExtension = URLParser::getFileExtension(filePath);

    Volume* volume = new UniformRectiLinearVolume();
    rawFile_ = filePath;
    
    //format_ = some format

    // Somewhere here open the QT DIALOG

    glm::mat3 basis(2.0f);
    glm::vec3 offset(0.0f);
    glm::vec3 spacing(0.0f);
    glm::mat4 wtm(1.0f);
  
    if(spacing != vec3(0.0f)) {
        basis[0][0] = dimension_.x * spacing.x;
        basis[1][1] = dimension_.y * spacing.y;
        basis[2][2] = dimension_.z * spacing.z;
    }

    // If not specified, center the data around origo.
    if(offset == vec3(0.0f)) {
        offset[0] = -basis[0][0] / 2.0f;
        offset[1] = -basis[1][1] / 2.0f;
        offset[2] = -basis[2][2] / 2.0f;
    }

    volume->setBasis(basis);
    volume->setOffset(offset);
    volume->setWorldTransform(wtm);
    volume->setDimension(dimension_);
    volume->setDataFormat(format_);
    VolumeDisk* vd = new VolumeDisk(filePath, dimension_, format_);
    vd->setDataReader(this);

    volume->addRepresentation(vd);

    std::string size = formatBytesToString(dimension_.x*dimension_.y*dimension_.z*(format_->getBytesStored()));

    LogInfo("Loaded volume: " << filePath << " size: " << size);

    return volume;
}



void RawVolumeReader::readDataInto(void* destination) const {
    std::fstream fin(rawFile_.c_str(), std::ios::in | std::ios::binary);
    if(fin.good()) {
        std::size_t size = dimension_.x*dimension_.y*dimension_.z*(format_->getBytesStored());
        fin.read(static_cast<char*>(destination), size);

        if(!littleEndian_ && format_->getBytesStored() > 1) {
            std::size_t bytes = format_->getBytesStored();
            char* temp = new char[bytes];

            for(std::size_t i = 0; i < size; i += bytes) {
                for(std::size_t j = 0; j < bytes; j++) {
                    temp[j] = static_cast<char*>(destination)[i + j];
                }
                for(std::size_t j = 0; j < bytes; j++) {
                    static_cast<char*>(destination)[i + j] = temp[bytes - j - 1];
                }
            }
            delete temp;
        }
    } else {
        throw DataReaderException("Error: Could not read from raw file: " + rawFile_);
    }
    fin.close();
}

void* RawVolumeReader::readData() const {
    std::size_t size = dimension_.x*dimension_.y*dimension_.z*(format_->getBytesStored());
    char* data = new char[size];
    if(data) {
        readDataInto(data);
    } else {
        throw DataReaderException("Error: Could not allocate memory for loading raw file: " + rawFile_);
    }
    return data;
}


} // namespace

