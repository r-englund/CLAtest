/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/core/io/datvolumereader.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>
#include <inviwo/core/util/urlparser.h>
#include <inviwo/core/util/formatconversion.h>
#include <inviwo/core/util/stringconversion.h>

namespace inviwo {

DatVolumeReader::DatVolumeReader()
    : DataReaderType<Volume>()
    , rawFile_("")
    , littleEndian_(true)
    , dimension_(uvec3(0, 0, 0))
    , format_(NULL) {
    addExtension(FileExtension("dat", "Inviwo dat file format"));
}

DatVolumeReader::DatVolumeReader(const DatVolumeReader& rhs)
    : DataReaderType<Volume>(rhs)
    , rawFile_(rhs.rawFile_)
    , littleEndian_(rhs.littleEndian_)
    , dimension_(rhs.dimension_)
    , format_(rhs.format_) {};

DatVolumeReader& DatVolumeReader::operator=(const DatVolumeReader& that) {
    if (this != &that) {
        rawFile_ = that.rawFile_;
        littleEndian_ = that.littleEndian_;
        dimension_ = that.dimension_;
        format_ = that.format_;
        DataReaderType<Volume>::operator=(that);
    }

    return *this;
}

DatVolumeReader* DatVolumeReader::clone() const { return new DatVolumeReader(*this); }

Volume* DatVolumeReader::readMetaData(std::string filePath) {
    if (!URLParser::fileExists(filePath)) {
        std::string newPath = URLParser::addBasePath(filePath);

        if (URLParser::fileExists(newPath)) {
            filePath = newPath;
        } else {
            throw DataReaderException("Error could not find input file: " + filePath);
        }
    }

    std::string fileDirectory = URLParser::getFileDirectory(filePath);
    std::string fileExtension = URLParser::getFileExtension(filePath);
    // Read the dat file content
    std::istream* f = new std::ifstream(filePath.c_str());
    std::string textLine;
    std::string formatFlag = "";
    Volume* volume = new UniformRectiLinearVolume();
    glm::mat3 basis(2.0f);
    glm::vec3 offset(0.0f);
    glm::vec3 spacing(0.0f);
    glm::mat4 wtm(1.0f);
    glm::vec3 a(0.0f), b(0.0f), c(0.0f);
    std::vector<std::string> parts;
    std::string key;
    std::string value;
    dvec2 datarange(0);
    dvec2 valuerange(0);
    std::string unit("");

    while (!f->eof()) {
        getline(*f, textLine);

        textLine = trim(textLine);
        if (textLine == "" || textLine[0] == '#' || textLine[0] == '/') continue;

        parts = splitString(textLine, ':');
        if (parts.size() != 2) continue;

        key = toLower(trim(parts[0]));
        value = trim(parts[1]);
        std::stringstream ss(value);

        if (key == "objectfilename" || key == "rawfile") {
            rawFile_ = fileDirectory + value;
        } else if (key == "byteorder") {
            if (toLower(value) == "bigendian") {
                littleEndian_ = false;
            } else {
                littleEndian_ = true;
            }
        } else if (key == "resolution" || key == "dimension") {
            ss >> dimension_.x;
            ss >> dimension_.y;
            ss >> dimension_.z;
        } else if (key == "spacing" || key == "slicethickness") {
            ss >> spacing.x;
            ss >> spacing.y;
            ss >> spacing.z;
        } else if (key == "basisvector1") {
            ss >> a.x;
            ss >> a.y;
            ss >> a.z;
        } else if (key == "basisvector2") {
            ss >> b.x;
            ss >> b.y;
            ss >> b.z;
        } else if (key == "basisvector3") {
            ss >> c.x;
            ss >> c.y;
            ss >> c.z;
        } else if (key == "offset") {
            ss >> offset.x;
            ss >> offset.y;
            ss >> offset.z;
        } else if (key == "worldvector1") {
            ss >> wtm[0][0];
            ss >> wtm[1][0];
            ss >> wtm[2][0];
            ss >> wtm[3][0];
        } else if (key == "worldvector2") {
            ss >> wtm[0][1];
            ss >> wtm[1][1];
            ss >> wtm[2][1];
            ss >> wtm[3][1];
        } else if (key == "worldvector3") {
            ss >> wtm[0][2];
            ss >> wtm[1][2];
            ss >> wtm[2][2];
            ss >> wtm[3][2];
        } else if (key == "worldvector4") {
            ss >> wtm[0][3];
            ss >> wtm[1][3];
            ss >> wtm[2][3];
            ss >> wtm[3][3];
        } else if (key == "format") {
            ss >> formatFlag;
            format_ = DataFormatBase::get(formatFlag);
        } else if (key == "datarange") {
            ss >> datarange.x;
            ss >> datarange.y;
        } else if (key == "valuerange") {
            ss >> valuerange.x;
            ss >> valuerange.y;
        } else if (key == "unit") {
            unit = value;
        } else {
            volume->setMetaData<StringMetaData>(key, value);
        }
    };

    delete f;

    if (dimension_ == uvec3(0))
        throw DataReaderException("Error: Unable to find \"Resolution\" tag in .dat file: " +
                                  filePath);
    else if (format_ == NULL)
        throw DataReaderException("Error: Unable to find \"Format\" tag in .dat file: " + filePath);
    else if (rawFile_ == "")
        throw DataReaderException("Error: Unable to find \"ObjectFilename\" tag in .dat file: " +
                                  filePath);

    if (spacing != vec3(0.0f)) {
        basis[0][0] = dimension_.x * spacing.x;
        basis[1][1] = dimension_.y * spacing.y;
        basis[2][2] = dimension_.z * spacing.z;
    }

    if (a != vec3(0.0f) && b != vec3(0.0f) && c != vec3(0.0f)) {
        basis[0][0] = a.x;
        basis[1][0] = a.y;
        basis[2][0] = a.z;
        basis[0][1] = b.x;
        basis[1][1] = b.y;
        basis[2][1] = b.z;
        basis[0][2] = c.x;
        basis[1][2] = c.y;
        basis[2][2] = c.z;
    }

    // If not specified, center the data around origo.
    if (offset == vec3(0.0f)) {
        offset[0] = -basis[0][0] / 2.0f;
        offset[1] = -basis[1][1] / 2.0f;
        offset[2] = -basis[2][2] / 2.0f;
    }

    volume->setBasis(basis);
    volume->setOffset(offset);
    volume->setWorldTransform(wtm);
    volume->setDimension(dimension_);

    volume->dataMap_.initWithFormat(format_);
    if (datarange != dvec2(0)) {
        volume->dataMap_.dataRange = datarange;
    }
    if (valuerange != dvec2(0)) {
        volume->dataMap_.valueRange = valuerange;
    } else {
        volume->dataMap_.valueRange = volume->dataMap_.dataRange;
    }
    if (unit != "") {
        volume->dataMap_.valueUnit = unit;
    }

    volume->setDataFormat(format_);

    VolumeDisk* vd = new VolumeDisk(filePath, dimension_, format_);
    vd->setDataReader(this);
    volume->addRepresentation(vd);
    std::string size = formatBytesToString(dimension_.x * dimension_.y * dimension_.z *
                                           (format_->getBytesAllocated()));
    LogInfo("Loaded volume: " << filePath << " size: " << size);
    return volume;
}

void DatVolumeReader::readDataInto(void* destination) const {
    std::fstream fin(rawFile_.c_str(), std::ios::in | std::ios::binary);

    if (fin.good()) {
        std::size_t size =
            dimension_.x * dimension_.y * dimension_.z * (format_->getBytesAllocated());
        fin.read(static_cast<char*>(destination), size);

        if (!littleEndian_ && format_->getBytesAllocated() > 1) {
            std::size_t bytes = format_->getBytesAllocated();
            char* temp = new char[bytes];

            for (std::size_t i = 0; i < size; i += bytes) {
                for (std::size_t j = 0; j < bytes; j++)
                    temp[j] = static_cast<char*>(destination)[i + j];

                for (std::size_t j = 0; j < bytes; j++)
                    static_cast<char*>(destination)[i + j] = temp[bytes - j - 1];
            }

            delete temp;
        }
    } else
        throw DataReaderException("Error: Could not read from raw file: " + rawFile_);

    fin.close();
}

void* DatVolumeReader::readData() const {
    std::size_t size = dimension_.x * dimension_.y * dimension_.z * (format_->getBytesAllocated());
    char* data = new char[size];

    if (data) {
        readDataInto(data);
    } else {
        throw DataReaderException("Error: Could not allocate memory for loading raw file: " +
                                  rawFile_);
    }

    return data;
}

}  // namespace
