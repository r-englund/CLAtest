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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/core/common/inviwocore.h>

//Data Structures
#include <inviwo/core/datastructures/volume/volumeramconverter.h>
#include <inviwo/core/datastructures/image/imageramconverter.h>

//Meta Data
#include <inviwo/core/metadata/metadata.h>
#include <inviwo/core/metadata/processormetadata.h>
#include <inviwo/core/metadata/processorwidgetmetadata.h>

//Utilizes
#include <inviwo/core/util/systemcapabilities.h>
#include <inviwo/core/util/vectoroperations.h>
#include <inviwo/core/util/settings/systemsettings.h>
#include <inviwo/core/util/settings/linksettings.h>

//Io
#include <inviwo/core/io/datvolumereader.h>
#include <inviwo/core/io/ivfvolumereader.h>
#include <inviwo/core/io/datvolumewriter.h>
#include <inviwo/core/io/ivfvolumewriter.h>

//Others
#include <inviwo/core/processors/canvasprocessor.h>
//Properties
#include <inviwo/core/properties/buttonproperty.h>


namespace inviwo {

InviwoCore::InviwoCore() : InviwoModule() {
    setIdentifier("Core");
    
    // Register Converters
    addRepresentationConverter(new VolumeDisk2RAMConverter());
    addRepresentationConverter(new ImageDisk2RAMConverter());

    // Register MetaData
    #define MetaDataMacro(n, t, d) addMetaData(new n##MetaData());
	#include <inviwo/core/metadata/metadatadefinefunc.h>

    addMetaData(new VectorMetaData<2,float>());
    addMetaData(new VectorMetaData<3,float>());
    addMetaData(new VectorMetaData<4,float>());

    addMetaData(new VectorMetaData<2,int>());
    addMetaData(new VectorMetaData<3,int>());
    addMetaData(new VectorMetaData<4,int>());

    addMetaData(new VectorMetaData<2,unsigned int>());
    addMetaData(new VectorMetaData<3,unsigned int>());
    addMetaData(new VectorMetaData<4,unsigned int>());

    addMetaData(new MatrixMetaData<2,float>());
    addMetaData(new MatrixMetaData<3,float>());
    addMetaData(new MatrixMetaData<4,float>());

    addMetaData(new PositionMetaData());
    addMetaData(new ProcessorMetaData());
    addMetaData(new ProcessorWidgetMetaData());

    // Register Capabilities
    addCapabilities(new SystemCapabilities());

    // Register Data readers
    addDataReader(new DatVolumeReader());
    addDataReader(new IvfVolumeReader());

    // Register Data writers
    addDataWriter(new DatVolumeWriter());
    addDataWriter(new IvfVolumeWriter());    

    // Register Settings
    addSettings(new SystemSettings(this));
    addSettings(new LinkSettings(this));
}

void InviwoCore::setupModuleSettings(){
    for (size_t i=0; i<moduleSettings_.size(); i++)
        moduleSettings_[i]->initialize();
}

} // namespace
