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
 * Contact: Erik Sundén
 *
 *********************************************************************************/

#include <modules/pvm/pvmvolumereader.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>
#include <inviwo/core/util/filesystem.h>
#include <inviwo/core/util/formatconversion.h>
#include <inviwo/core/util/stringconversion.h>
#include "ext/tidds/ddsbase.h"

namespace inviwo {

PVMVolumeReader::PVMVolumeReader()
    : DataReaderType<Volume>()
    , rawFile_("")
    , filePos_(0)
    , littleEndian_(true)
    , dimension_(uvec3(0, 0, 0))
    , format_(NULL) {
    addExtension(FileExtension("pvm", "PVM file format"));
}

PVMVolumeReader::PVMVolumeReader(const PVMVolumeReader& rhs)
    : DataReaderType<Volume>(rhs)
    , rawFile_(rhs.rawFile_)
    , filePos_(rhs.filePos_)
    , littleEndian_(rhs.littleEndian_)
    , dimension_(rhs.dimension_)
    , format_(rhs.format_) {};

PVMVolumeReader& PVMVolumeReader::operator=(const PVMVolumeReader& that) {
    if (this != &that) {
        rawFile_ = that.rawFile_;
        filePos_ = that.filePos_;
        littleEndian_ = that.littleEndian_;
        dimension_ = that.dimension_;
        format_ = that.format_;
        DataReaderType<Volume>::operator=(that);
    }

    return *this;
}

PVMVolumeReader* PVMVolumeReader::clone() const { return new PVMVolumeReader(*this); }

Volume* PVMVolumeReader::readMetaData(std::string filePath) {
    if (!filesystem::fileExists(filePath)) {
        std::string newPath = filesystem::addBasePath(filePath);

        if (filesystem::fileExists(newPath)) {
            filePath = newPath;
        }
        else {
            throw DataReaderException("Error could not find input file: " + filePath);
        }
    }

    
    Volume* volume = new UniformRectiLinearVolume();
    glm::mat3 basis(2.0f);
    glm::vec3 spacing(0.0f);
    glm::vec3 a(0.0f), b(0.0f), c(0.0f);

    //Check PVM version
    std::istream* f = new std::ifstream(filePath.c_str());
    std::string textLine;
    getline(*f, textLine);
    textLine = trim(textLine);

    int pvmVersion = 0;
    int ddsVersion = 0;

    if (textLine == "PVM")
        pvmVersion = 1;
    else if (textLine == "PVM2")
        pvmVersion = 2;
    else if (textLine == "PVM3")
        pvmVersion = 3;
    else if (textLine == "DDS v3d"){
        pvmVersion = 3;
        ddsVersion = 1;
    }
    else if (textLine == "DDS v3e"){
        pvmVersion = 3;
        ddsVersion = 2;
    }

    delete f;

    if (pvmVersion == 0)
        throw DataReaderException("Error: Unsupported PVM version " + textLine + " in file: " +
        filePath);

    // Reading PVM volume
    unsigned char* data = NULL;
    unsigned int bytesPerVoxel;
    unsigned char *description;
    unsigned char *courtesy;
    unsigned char *parameter;
    unsigned char *comment;

    data = readPVMvolume(filePath.c_str(), &dimension_.x, &dimension_.y, &dimension_.z,
        &bytesPerVoxel, &spacing.x, &spacing.y, &spacing.z, &description, &courtesy,
        &parameter, &comment);

    if (data == NULL)
        throw DataReaderException("Error: Could not read data in PVM file: " +
        filePath);

    format_ = DataFormatBase::get(DataFormatEnums::UNSIGNED_INTEGER_TYPE, 1, bytesPerVoxel * 8);

    if (format_ == NULL)
        throw DataReaderException("Error: Unable to find bytes per voxel in .pvm file: " + filePath);

    if (dimension_ == uvec3(0))
        throw DataReaderException("Error: Unable to find dimensions in .pvm file: " +
        filePath);

    if (format_ == DataUINT16::get()){
        DataUINT16::type* d = reinterpret_cast<DataUINT16::type*>(data);
        DataUINT16::type m = 0;
        for (int i = 0; i < dimension_.x*dimension_.y*dimension_.z; i++) {
            d[i] = (d[i] >> 8) | (d[i] << 8);
            if (d[i] > m)
                m = d[i];
        }
        if (m <= DataUINT12::max()) {
            format_ = DataUINT12::get();
        }
    }

    // Additional information
    if (pvmVersion > 2){
        std::stringstream ss;

        if (description){
            ss << description;
            volume->setMetaData<StringMetaData>("Description", ss.str());
        }

        if (courtesy){
            ss.clear();
            ss << courtesy;
            volume->setMetaData<StringMetaData>("Courtesy", ss.str());
        }

        if (parameter){
            ss.clear();
            ss << parameter;
            volume->setMetaData<StringMetaData>("Parameter", ss.str());
        }

        if (comment){
            ss.clear();
            ss << comment;
            volume->setMetaData<StringMetaData>("Comment", ss.str());
        }
    }

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

    volume->setBasis(basis);
    volume->setDimension(dimension_);

    volume->dataMap_.initWithFormat(format_);

    volume->setDataFormat(format_);

    // Create RAM volume s all data has already is in memory
    VolumeRAM* volRAM = createVolumeRAM(dimension_, format_, data);
    volume->addRepresentation(volRAM);

    // Print information
    size_t bytes = dimension_.x * dimension_.y * dimension_.z * (format_->getBytesAllocated());
    std::string size = formatBytesToString(bytes);
    LogInfo("Loaded volume: " << filePath << " size: " << size);
    if (pvmVersion > 2){
        if (description) LogInfo("Description: " << volume->getMetaData<StringMetaData>("Description")->get());
        if (courtesy) LogInfo("Courtesy: " << volume->getMetaData<StringMetaData>("Courtesy")->get());
        if (parameter) LogInfo("Parameter: " << volume->getMetaData<StringMetaData>("Parameter")->get());
        if (comment) LogInfo("Comment: " << volume->getMetaData<StringMetaData>("Comment")->get());
    }

    return volume;
}

void PVMVolumeReader::readDataInto(void*) const {
    return;
}

void* PVMVolumeReader::readData() const {
    return NULL;
}

}  // namespace
