#include "inviwo/core/inviwocore.h"

namespace inviwo {

    InviwoCore::InviwoCore() : InviwoModule() {
        setIdentifier("Core");
        //setXMLFileName(InviwoApplication::app()->getRootPath() + "/src/core/core.xml", true);
    }

} // namespace
