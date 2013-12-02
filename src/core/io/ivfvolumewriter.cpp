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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/io/ivfvolumewriter.h>
#include <inviwo/core/io/rawvolumewriter.h>

namespace inviwo {


IvfVolumeWriter::IvfVolumeWriter(const std::string filePath, IvfWriterSettings writerSettings) 
    : VolumeWriter()
    , sourceFileAbsolutePath_(filePath)
    , writerFileSettings_(writerSettings)
{}

void IvfVolumeWriter::writeData() {    
    writeRawVolumeData();
}    

void IvfVolumeWriter::writeRawVolumeData() {
    //Check if raw data
    const void* rawData = writerFileSettings_.texels_;
    ivwAssert(rawData!=0, "raw data does not exist");

    //Write Dat file settings
    IvfVolumeWriter::writeIvfFileSettings(sourceFileAbsolutePath_, writerFileSettings_);    
    RawVolumeWriter::saveRawData(writerFileSettings_);
}

} // namespace
