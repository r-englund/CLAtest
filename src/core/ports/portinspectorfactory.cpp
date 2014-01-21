#include <inviwo/core/ports/portinspectorfactory.h>

namespace inviwo {

PortInspectorFactory::PortInspectorFactory() {
    initialize();
}


void PortInspectorFactory::initialize() {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
        std::vector<PortInspector*> curList = inviwoApp->getModules()[curModuleId]->getPortInspectors();
        for (size_t i=0; i<curList.size(); i++){
            registerPortInspector(curList[i]);
        }
    }
}

void PortInspectorFactory::registerPortInspector(PortInspector* portInspector){
    
    std::string className = portInspector->getPortClassName();
    
    if(portInspectors_.find(className) == portInspectors_.end()){
        portInspectors_.insert(std::make_pair(className, portInspector));
    }else{
        LogWarn("PortInspector for " << className << " already registered");
    }
}

PortInspector* PortInspectorFactory::getPortInspectorForPortClass(std::string className){
    PortInspectorMap::iterator it = portInspectors_.find(className);
    if (it != portInspectors_.end()){
        return it->second;
    }else{
        return NULL;
    }
}

} // namespace

