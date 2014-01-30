#include <inviwo/core/ports/portfactory.h>

namespace inviwo {

PortFactory::PortFactory() {}

PortFactory::~PortFactory() {}


void PortFactory::registeryObject(PortFactoryObject* port) {
    std::string className = port->getClassName();

    PortClassMap::const_iterator it = portClassMap_.find(className);
    if(it == portClassMap_.end()) {
        portClassMap_.insert(std::make_pair(className, port));;

    } else {
        LogWarn("Port with class name: " << className << " already registed");
    }
}

IvwSerializable* PortFactory::create(std::string className) const {
    return NULL;
}

bool PortFactory::isValidType(std::string className) const {
    PortClassMap::const_iterator it = portClassMap_.find(className);
    if(it != portClassMap_.end()) {
        return true;
    } else {
        return false;
    }
}

Port* PortFactory::getPort(std::string className, std::string identifier) {
    PortClassMap::const_iterator it = portClassMap_.find(className);
    if(it != portClassMap_.end()) {
        return it->second->create(identifier);
    } else {
        return NULL;
    }
}

std::vector<std::string> PortFactory::getRegistedPortClassNames() {
    std::vector<std::string> classNames;
    for(PortClassMap::iterator it = portClassMap_.begin(); it != portClassMap_.end(); ++it) {
        classNames.push_back(it->first);
    }
    return classNames;
}

} // namespace

