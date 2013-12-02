/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/io/rawvolumewriter.h>
#include <inviwo/core/io/datvolumewriter.h>
#include <inviwo/core/io/ivfvolumewriter.h>

namespace inviwo {

VolumeRAM::VolumeRAM(uvec3 dimensions, VolumeRepresentation::VolumeBorders border, const DataFormatBase* format)
    : VolumeRepresentation(dimensions, format, border), data_(0)
{}

VolumeRAM::~VolumeRAM() {
    deinitialize();
}

void VolumeRAM::initialize() {}

void VolumeRAM::deinitialize() {
    // Make sure that data is deinitialized in
    // child class (should not delete void pointer 
    // since destructor will not be called for object.
}

void* VolumeRAM::getData() {
    return data_;
}

const void* VolumeRAM::getData() const {
    return const_cast<void*>(data_);
}

void VolumeRAM::saveData(std::string url) const {
    std::string fileExtension = URLParser::getFileExtension(url);
    if (!fileExtension.empty()) {
        //TODO: better pattern for automatic data writer selection
        if (fileExtension=="dat") {
            WriterSettings writerSettings;
            writerSettings.rawFileAbsolutePath_ = URLParser::replaceFileExtension(url, "raw");
            writerSettings.dimensions_ = dimensions_;
            writerSettings.dataFormat_ = std::string(getDataFormat()->getString());
            writerSettings.texels_ = getData();
            DatVolumeWriter::writeDatFileSettings(url, writerSettings);
            RawVolumeWriter::saveRawData(writerSettings);
        }
        else if (fileExtension=="ivf") {
            IvfWriterSettings writerSettings;
            writerSettings.rawFileAbsolutePath_ = URLParser::replaceFileExtension(url, "raw");
            writerSettings.dimensions_ = dimensions_;
            writerSettings.dataFormat_ = std::string(getDataFormat()->getString());
            writerSettings.texels_ = getData();
            IvfVolumeWriter::writeIvfFileSettings(url, writerSettings);
            RawVolumeWriter::saveRawData(writerSettings);
        }
    }
}

VolumeRAM* createVolumeRAM(const uvec3& dimension, const DataFormatBase* format) {
    // TODO: Add more formats
    VolumeRAM* result = 0;
    switch (format->getId())
    {
    case NOT_SPECIALIZED:
        LogErrorCustom("createVolumeRAM", "Invalid format");
    #define DataFormatIdMacro(i) case i: return new VolumeRAMCustomPrecision<Data##i::type, Data##i::bits>(dimension); break;
    #include <inviwo/core/util/formatsdefinefunc.h>
    default:
        LogErrorCustom("createVolumeRAM", "Invalid format or not implemented");
        break;
    }
    return result;
}

} // namespace

