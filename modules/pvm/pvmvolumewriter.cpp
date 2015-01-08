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

#include <modules/pvm/pvmvolumewriter.h>
#include <inviwo/core/util/filesystem.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include "ext/tidds/ddsbase.h"

namespace inviwo {

PVMVolumeWriter::PVMVolumeWriter() : DataWriterType<Volume>() {
    addExtension(FileExtension("pvm", "PVM file format"));
}

PVMVolumeWriter::PVMVolumeWriter(const PVMVolumeWriter& rhs) : DataWriterType<Volume>(rhs) {
}

PVMVolumeWriter& PVMVolumeWriter::operator=(const PVMVolumeWriter& that) {
    if (this != &that)
        DataWriterType<Volume>::operator=(that);

    return *this;
}

PVMVolumeWriter* PVMVolumeWriter::clone() const {
    return new PVMVolumeWriter(*this);
}

void PVMVolumeWriter::writeData(const Volume* data, const std::string filePath) const {
    if (filesystem::fileExists(filePath)  && !overwrite_)
        throw DataWriterException("Error: Output file: " + filePath + " already exists");

    const DataFormatBase* format = data->getDataFormat();
    int components = 0;
    switch (format->getId())
    {
    case inviwo::DataFormatEnums::UINT8:
        components = 1;
    case inviwo::DataFormatEnums::UINT12:
    case inviwo::DataFormatEnums::UINT16:
        components = 2;
        break;
    default:
        break;
    }

    if(components == 0)
        throw DataWriterException("Error: Output format " + std::string(format->getString()) + " not support by PVM writer");

    const VolumeRAM* vr = data->getRepresentation<VolumeRAM>();

    unsigned const char *dataPtr = (unsigned const char *)vr->getData();;
    if(components == 2){
        //TODO: Might need swap fixes for PVM writing
        /*DataUINT16::type* d = reinterpret_cast<DataUINT16::type*>(data);
        DataUINT16::type m = 0;
        for (int i = 0; i < dim.x*dim.y*dim.z; i++) {
            d[i] = (d[i] >> 8) | (d[i] << 8);
        }*/
    }

    uvec3 dim = vr->getDimension();
    uvec3 scale(1.f);

    unsigned const char *description = NULL;
    StringMetaData* descMetaData = data->getMetaData<StringMetaData>("Description");
    if(descMetaData)
        description = (unsigned const char *)descMetaData->get().c_str();

    unsigned const char *courtesy = NULL;
    StringMetaData* courMetaData = data->getMetaData<StringMetaData>("Courtesy");
    if(courMetaData)
        courtesy = (unsigned const char *)courMetaData->get().c_str();

    unsigned const char *parameter = NULL;
    StringMetaData* paraMetaData = data->getMetaData<StringMetaData>("Parameter");
    if(paraMetaData)
        parameter = (unsigned const char *)paraMetaData->get().c_str();

    unsigned const char *comment = NULL;
    StringMetaData* commMetaData = data->getMetaData<StringMetaData>("Comment");
    if(commMetaData)
        comment = (unsigned const char *)commMetaData->get().c_str();

    writePVMvolume(filePath.c_str(), dataPtr,
        dim.x,dim.y,dim.z, components,
        scale.x, scale.y, scale.z,
        description,
        courtesy,
        parameter,
        comment);
}

} // namespace
