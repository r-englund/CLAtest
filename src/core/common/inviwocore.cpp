/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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

#include <inviwo/core/common/inviwocore.h>

//Data Structures
#include <inviwo/core/datastructures/volume/volumeramconverter.h>
#include <inviwo/core/datastructures/image/layerramconverter.h>

//Meta Data
#include <inviwo/core/metadata/metadata.h>
#include <inviwo/core/metadata/processormetadata.h>
#include <inviwo/core/metadata/processorwidgetmetadata.h>
#include <inviwo/core/metadata/propertyeditorwidgetmetadata.h>

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
#include <inviwo/core/io/rawvolumereader.h>

//Others
#include <inviwo/core/processors/canvasprocessor.h>

//Ports
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>

//PortInspectors
#include <inviwo/core/ports/portinspector.h>

//Properties
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/directoryproperty.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/properties/imageeditorproperty.h>
#include <inviwo/core/properties/minmaxproperty.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/stringproperty.h>
#include <inviwo/core/properties/transferfunctionproperty.h>


namespace inviwo {

InviwoCore::InviwoCore() : InviwoModule() {
    setIdentifier("Core");
    // Register Converters
    registerRepresentationConverter(new VolumeDisk2RAMConverter());
    registerRepresentationConverter(new LayerDisk2RAMConverter());
    // Register MetaData
#define MetaDataMacro(n, t, d, v) registerMetaData(new n##MetaData());
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
    registerMetaData(new PropertyEditorWidgetMetaData());
    // Register Capabilities
    registerCapabilities(new SystemCapabilities());
    // Register Data readers
    registerDataReader(new DatVolumeReader());
    registerDataReader(new IvfVolumeReader());
    registerDataReader(new RawVolumeReader());
    // Register Data writers
    registerDataWriter(new DatVolumeWriter());
    registerDataWriter(new IvfVolumeWriter());
    // Register Settings
    registerSettings(new SystemSettings());
    registerSettings(new LinkSettings());
    // Register Ports
    registerPort(GeometryInport);
    registerPort(GeometryMultiInport);
    registerPort(GeometryOutport);
    registerPort(ImageInport);
    registerPort(ImageOutport);
    registerPort(VolumeInport);
    registerPort(VolumeOutport);
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
    registerProperty(CameraProperty);
    registerProperty(TransferFunctionProperty);
    registerProperty(BoolProperty);
    registerProperty(ButtonProperty);
    registerProperty(DirectoryProperty);
    //registerProperty(EventProperty); TODO fix "default" contructor with 2 args...
    registerProperty(FileProperty);
    registerProperty(ImageEditorProperty);
    registerProperty(FloatMat2Property);
    registerProperty(FloatMat3Property);
    registerProperty(FloatMat4Property);
    registerProperty(OptionPropertyInt);
    registerProperty(OptionPropertyFloat);
    registerProperty(OptionPropertyDouble);
    registerProperty(OptionPropertyString);
    registerProperty(FloatProperty);
    registerProperty(IntProperty);
    registerProperty(DoubleProperty);
    registerProperty(StringProperty);
    registerProperty(FloatMinMaxProperty);
    registerProperty(FloatVec2Property);
    registerProperty(FloatVec3Property);
    registerProperty(FloatVec4Property);
    registerProperty(DoubleVec2Property);
    registerProperty(DoubleVec3Property);
    registerProperty(DoubleVec4Property);
    registerProperty(IntMinMaxProperty);
    registerProperty(IntVec2Property);
    registerProperty(IntVec3Property);
    registerProperty(IntVec4Property);
}

} // namespace
