#include "inviwo/core/inviwocore.h"

#include "inviwo/core/processors/canvasprocessor.h"
#include "inviwo/core/processors/datasource/volumesource.h"

namespace inviwo {

    InviwoCore::InviwoCore() : InviwoModule() {
        setIdentifier("Core");
        //setXMLFileName(InviwoApplication::app()->getRootPath() + "/src/core/core.xml", true);

        addProcessor(new VolumeSource());
    }

} // namespace
