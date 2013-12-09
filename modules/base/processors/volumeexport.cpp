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

#include "volumeexport.h"
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/io/datawriter.h>
#include <inviwo/core/io/datawriterfactory.h>
#include <inviwo/core/util/filedirectory.h>
#include <inviwo/core/util/fileextension.h>

namespace inviwo {

ProcessorClassName(VolumeExport, "VolumeExport"); 
ProcessorCategory(VolumeExport, "Data Output");
ProcessorCodeState(VolumeExport, CODE_STATE_EXPERIMENTAL);

VolumeExport::VolumeExport()
    : Processor()
    , volumePort_("volume")
    , volumeFile_("volumeFileName", "Volume file name", IVW_DIR+"data/volumes/newvolume.dat")
    , exportVolumeButton_("snapshot", "Export Volume", PropertyOwner::VALID)
    , overwrite_("overwrite", "Overwrite", false){

    std::vector<FileExtension> ext = DataWriterFactory::getRef().getExtensionsForType<Volume>();
    for(std::vector<FileExtension>::const_iterator it = ext.begin();
        it != ext.end(); ++it){
            std::stringstream ss;
            ss << it->description_ << " (*." << it->extension_ << ")";
            volumeFile_.addNameFilter(ss.str());
    }
    addPort(volumePort_);
    addProperty(volumeFile_);
    volumeFile_.setAcceptMode(FileProperty::AcceptSave);
    exportVolumeButton_.onChange(this, &VolumeExport::exportVolume);
    addProperty(exportVolumeButton_);
    addProperty(overwrite_);
}

VolumeExport::~VolumeExport() {}

void VolumeExport::initialize() {
    Processor::initialize();    
}

void VolumeExport::deinitialize() {
    Processor::deinitialize();
}

void VolumeExport::exportVolume() {
    //TODO: if volume has changed setData
    const Volume* volume = volumePort_.getData();
    if (volume && !volumeFile_.get().empty()) {

        std::string fileExtension = URLParser::getFileExtension(volumeFile_.get());
        DataWriterType<Volume>* writer = 
            DataWriterFactory::getRef().getWriterForTypeAndExtension<Volume>(fileExtension);
        if(writer){
            try{
                writer->setOverwrite(overwrite_.get());
                writer->writeData(volume, volumeFile_.get());
                LogInfo("Volume exported to disk: " << volumeFile_.get());
            }catch(DataWriterException const& e){
                LogError(e.getMessage());
            }
        }else{
            LogError("Error: Cound not find a writer for the specified extension and data type");
        }
    } else if (volumeFile_.get().empty()){
        LogError("Error: Please specify a file to write to");
    } else if (!volume){
        LogError("Error: Please connect a volume to export");
    }
}

void VolumeExport::process() {}

} // namespace
