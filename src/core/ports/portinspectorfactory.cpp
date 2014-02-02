#include <inviwo/core/ports/portinspectorfactory.h>
#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

PortInspectorFactory::PortInspectorFactory() {}

PortInspectorFactory::~PortInspectorFactory() {}

void PortInspectorFactory::registerObject(PortInspector* portInspector) {
    std::string className = portInspector->getPortClassName();

    if (portInspectors_.find(className) == portInspectors_.end())
        portInspectors_.insert(std::make_pair(className, portInspector));
    else
        LogWarn("PortInspector for " << className << " already registered");
}

PortInspector* PortInspectorFactory::getPortInspectorForPortClass(std::string className) {
    PortInspectorMap::iterator it = portInspectors_.find(className);

    if (it != portInspectors_.end())
        return it->second;
    else
        return NULL;
}

bool PortInspectorFactory::isValidType(std::string className) const {
    PortInspectorMap::const_iterator it = portInspectors_.find(className);

    if (it != portInspectors_.end())
        return true;
    else
        return false;
}



} // namespace

