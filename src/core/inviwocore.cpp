#include "inviwo/core/inviwocore.h"

#include "inviwo/core/processors/canvasprocessor.h"
#include "inviwo/core/processors/datasource/volumesource.h"

#include "inviwo/core/metadata/positionmetadata.h"

#include "inviwo/core/datastructures/volumeramconverter.h"

namespace inviwo {

    InviwoCore::InviwoCore() : InviwoModule() {
        setIdentifier("Core");
        //setXMLFileName(InviwoApplication::app()->getRootPath() + "/src/core/core.xml", true);

        addProcessor(new VolumeSource());

        addRepresentationConverter(new VolumeRAMConverter());

        addMetaData(new PositionMetaData());
    }

} // namespace
