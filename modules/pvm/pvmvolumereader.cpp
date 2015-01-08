/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2015 Inviwo Foundation
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
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>
#include <inviwo/core/util/filesystem.h>
#include <inviwo/core/util/formatconversion.h>
#include <inviwo/core/util/stringconversion.h>
#include "ext/tidds/ddsbase.h"

namespace inviwo {

PVMVolumeReader::PVMVolumeReader()
    : DataReaderType<Volume>() {
    addExtension(FileExtension("pvm", "PVM file format"));
}

PVMVolumeReader::PVMVolumeReader(const PVMVolumeReader& rhs)
    : DataReaderType<Volume>(rhs) {};

PVMVolumeReader& PVMVolumeReader::operator=(const PVMVolumeReader& that) {
    if (this != &that) {
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

    glm::uvec3 dim(0);
    glm::mat3 basis(2.0f);
    glm::vec3 spacing(0.0f);

    // Reading PVM volume
    unsigned char* data = NULL;
    unsigned int bytesPerVoxel;
    unsigned char *description;
    unsigned char *courtesy;
    unsigned char *parameter;
    unsigned char *comment;

    data = readPVMvolume(filePath.c_str(), &dim.x, &dim.y, &dim.z,
        &bytesPerVoxel, &spacing.x, &spacing.y, &spacing.z, &description, &courtesy,
        &parameter, &comment);

    if (data == NULL)
        throw DataReaderException("Error: Could not read data in PVM file: " +
        filePath);

    const DataFormatBase* format = DataFormatBase::get(DataFormatEnums::UNSIGNED_INTEGER_TYPE, 1, bytesPerVoxel * 8);

    if (format == NULL)
        throw DataReaderException("Error: Unable to find bytes per voxel in .pvm file: " + filePath);

    if (dim == uvec3(0))
        throw DataReaderException("Error: Unable to find dimensions in .pvm file: " +
        filePath);

    if (format == DataUINT16::get()){
        size_t bytes = format->getBytesAllocated();
        size_t size = dim.x*dim.y*dim.z*bytes;
        swapbytes(data, static_cast<unsigned int>(size));
        DataUINT16::type m = 0, c = 0;
        for (size_t i = 0; i < size; i += bytes) {
            c = (data[i+1] << 8) | data[i];
            if (c > m) m = c;
        }
        if (m <= DataUINT12::max()) {
            format = DataUINT12::get();
        }
    }

    Volume* volume = new UniformRectiLinearVolume();

    // Additional information
    std::stringstream ss;

    if (description){
        ss << description;
        volume->setMetaData<StringMetaData>("description", ss.str());
    }

    if (courtesy){
        ss.clear();
        ss.str("");
        ss << courtesy;
        volume->setMetaData<StringMetaData>("courtesy", ss.str());
    }

    if (parameter){
        ss.clear();
        ss.str("");
        ss << parameter;
        volume->setMetaData<StringMetaData>("parameter", ss.str());
    }

    if (comment){
        ss.clear();
        ss.str("");
        ss << comment;
        volume->setMetaData<StringMetaData>("comment", ss.str());
    }

    if (spacing != vec3(0.0f)) {
        basis[0][0] = dim.x * spacing.x;
        basis[1][1] = dim.y * spacing.y;
        basis[2][2] = dim.z * spacing.z;
    }

    volume->setBasis(basis);
    volume->setOffset(-0.5f*(basis[0]+basis[1]+basis[2]));
    volume->setDimension(dim);

    volume->dataMap_.initWithFormat(format);

    volume->setDataFormat(format);

    // Create RAM volume s all data has already is in memory
    VolumeRAM* volRAM = createVolumeRAM(dim, format, data);
    volume->addRepresentation(volRAM);

    // Print information
    size_t bytes = dim.x * dim.y * dim.z * (format->getBytesAllocated());
    std::string size = formatBytesToString(bytes);
    LogInfo("Loaded volume: " << filePath << " size: " << size);
    printMetaInfo(volume, "description");
    printMetaInfo(volume, "courtesy");
    printMetaInfo(volume, "parameter");
    printMetaInfo(volume, "comment");

    return volume;
}

void PVMVolumeReader::readDataInto(void*) const {
    return;
}

void* PVMVolumeReader::readData() const {
    return NULL;
}

void PVMVolumeReader::printMetaInfo(MetaDataOwner* metaDataOwner, std::string key){
    StringMetaData* metaData = metaDataOwner->getMetaData<StringMetaData>(key);
    if (metaData){
        std::string metaStr = metaData->get();
        replaceInString(metaStr, "\n", ", ");
        key[0] = toupper(key[0]);
        LogInfo(key << ": " << metaStr);
    }
}

}  // namespace
