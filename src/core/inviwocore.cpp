#include "inviwo/core/inviwocore.h"

//Data Structures
#include "inviwo/core/datastructures/volumeramconverter.h"

//Meta Data
#include "inviwo/core/metadata/positionmetadata.h"
#include "inviwo/core/metadata/processorwidgetmetadata.h"

//Others
#include "inviwo/core/processors/canvasprocessor.h"
#include "inviwo/core/processors/datasource/volumesource.h"

namespace inviwo {

    InviwoCore::InviwoCore() : InviwoModule() {
        setIdentifier("Core");
        //setXMLFileName(InviwoApplication::app()->getRootPath() + "/src/core/core.xml", true);

        addProcessor(new VolumeSource());

        addRepresentationConverter(new VolumeDisk2RAMConverter());

        addMetaData(new PositionMetaData());
        addMetaData(new ProcessorWidgetMetaData());
    }

} // namespace
