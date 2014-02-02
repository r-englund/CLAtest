/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#include <inviwo/core/io/ivfvolumereader.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>
#include "inviwo/core/datastructures/volume/volumedisk.h"
#include "inviwo/core/util/urlparser.h"

namespace inviwo {

IvfVolumeReader::IvfVolumeReader()
    : DataReaderType<Volume>()
    , rawFile_("")
    , littleEndian_(true)
    , dimension_(uvec3(0))
    , format_(NULL) {
    addExtension(FileExtension("ivf", "Inviwo ivf file format"));
}

IvfVolumeReader::IvfVolumeReader(const IvfVolumeReader& rhs)
    : DataReaderType<Volume>(rhs)
    , rawFile_(rhs.rawFile_)
    , littleEndian_(true)
    , dimension_(rhs.dimension_)
    , format_(rhs.format_) {
}

IvfVolumeReader& IvfVolumeReader::operator=(const IvfVolumeReader& that) {
    if (this != &that) {
        rawFile_ = that.rawFile_;
        littleEndian_ = that.littleEndian_;
        dimension_ = that.dimension_;
        format_ = that.format_;
        DataReaderType<Volume>::operator=(that);
    }

    return *this;
}

IvfVolumeReader* IvfVolumeReader::clone() const {
    return new IvfVolumeReader(*this);
}

Volume* IvfVolumeReader::readMetaData(std::string filePath)  {
    if (!URLParser::fileExists(filePath)) {
        filePath = URLParser::addBasePath(filePath);

        if (!URLParser::fileExists(filePath))
            throw DataReaderException("Error: Input file: " + filePath + " does not exist");
    }

    std::string fileDirectory = URLParser::getFileDirectory(filePath);
    std::string fileExtension = URLParser::getFileExtension(filePath);
    Volume* volume = new UniformRectiLinearVolume();
    IvwDeserializer d(filePath);
    d.deserialize("ObjectFileName", rawFile_);
    rawFile_ = fileDirectory + rawFile_;
    std::string formatFlag("");
    d.deserialize("Format", formatFlag);
    format_ = DataFormatBase::get(formatFlag);
    mat4 basisAndOffset;
    d.deserialize("BasisAndOffset", basisAndOffset);
    volume->setBasisAndOffset(basisAndOffset);
    mat4 worldTransform;
    d.deserialize("WorldTransform", worldTransform);
    volume->setWorldTransform(worldTransform);
    d.deserialize("Dimension", dimension_);
    volume->setDimension(dimension_);
    volume->getMetaDataMap()->deserialize(d);
    littleEndian_ = volume->getMetaData<BoolMetaData>("LittleEndian", littleEndian_);
    VolumeDisk* vd = new VolumeDisk(filePath, dimension_, format_);
    vd->setDataReader(this);
    volume->addRepresentation(vd);
    return volume;
}

void IvfVolumeReader::readDataInto(void* destination) const {
    std::fstream fin(rawFile_.c_str(), std::ios::in | std::ios::binary);

    if (fin.good()) {
        std::size_t size = dimension_.x*dimension_.y*dimension_.z*(format_->getBytesStored());
        fin.read((char*)destination, size);

        if (!littleEndian_ && format_->getBytesStored() > 1) {
            std::size_t bytes = format_->getBytesStored();
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

void* IvfVolumeReader::readData() const {
    std::size_t size = dimension_.x*dimension_.y*dimension_.z*(format_->getBytesStored());
    char* data = new char[size];

    if (data)
        readDataInto(data);
    else
        throw DataReaderException("Error: Could not allocate memory for loading raw file: " + rawFile_);

    return data;
}


} // namespace
