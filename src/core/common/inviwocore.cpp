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
#include <inviwo/core/datastructures/image/layerramconverter.h>

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

//PortInspectors
#include <inviwo/core/ports/portinspector.h>

//Properties
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <inviwo/core/properties/optionproperties.h>


namespace inviwo {

InviwoCore::InviwoCore() : InviwoModule() {
    setIdentifier("Core");
    
    // Register Converters
    registerRepresentationConverter(new VolumeDisk2RAMConverter());
    registerRepresentationConverter(new LayerDisk2RAMConverter());

    // Register MetaData
    #define MetaDataMacro(n, t, d) registerMetaData(new n##MetaData());
	#include <inviwo/core/metadata/metadatadefinefunc.h>

    registerMetaData(new VectorMetaData<2,float>());
    registerMetaData(new VectorMetaData<3,float>());
    registerMetaData(new VectorMetaData<4,float>());

    registerMetaData(new VectorMetaData<2,int>());
    registerMetaData(new VectorMetaData<3,int>());
    registerMetaData(new VectorMetaData<4,int>());

    registerMetaData(new VectorMetaData<2,unsigned int>());
    registerMetaData(new VectorMetaData<3,unsigned int>());
    registerMetaData(new VectorMetaData<4,unsigned int>());

    registerMetaData(new MatrixMetaData<2,float>());
    registerMetaData(new MatrixMetaData<3,float>());
    registerMetaData(new MatrixMetaData<4,float>());

    registerMetaData(new PositionMetaData());
    registerMetaData(new ProcessorMetaData());
    registerMetaData(new ProcessorWidgetMetaData());

    // Register Capabilities
    registerCapabilities(new SystemCapabilities());

    // Register Data readers
    registerDataReader(new DatVolumeReader());
    registerDataReader(new IvfVolumeReader());

    // Register Data writers
    registerDataWriter(new DatVolumeWriter());
    registerDataWriter(new IvfVolumeWriter());    

    // Register Settings
    registerSettings(new SystemSettings());
    registerSettings(new LinkSettings());

    // Register Ports
        
    // Register PortInspectors
    registerPortInspector(new PortInspector("ImageOutport",
        InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_WORKSPACES)
        + "portinspectors/imageportinspector.inv"));
    
    registerPortInspector(new PortInspector("VolumeOutport",
        InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_WORKSPACES)
        + "portinspectors/volumeportinspector.inv"));
    
    registerPortInspector(new PortInspector("GeometryOutport",
        InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_WORKSPACES)
        + "portinspectors/geometryportinspector.inv"));
    
    // Register Properties
    // TODO: Register all properties
    registerProperty(new CameraProperty("camera", "Camera"));
    registerProperty(new TransferFunctionProperty("transferfunction", "TransferFunction"));
    registerProperty(new StringOptionProperty("stringoptions", "StringOption"));
}

void InviwoCore::setupModuleSettings(){
    for (size_t i=0; i<moduleSettings_.size(); i++)
        moduleSettings_[i]->initialize();
}

} // namespace
