#ifndef IVW_PORTFACTORYOBJECT_H
#define IVW_PORTFACTORYOBJECT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/port.h>

namespace inviwo {

class IVW_CORE_API PortFactoryObject { 
public:
    PortFactoryObject(const std::string& className);
    virtual ~PortFactoryObject();

    virtual Port* create(std::string identifier) = 0;

    std::string getClassName() const;

private:
    std::string className_;

};


template<typename T>
class PortFactoryObjectTemplate : public PortFactoryObject {
public:
    PortFactoryObjectTemplate(const std::string& className)
        : PortFactoryObject(className) {}

    virtual ~PortFactoryObjectTemplate() {}

    virtual Port* create(std::string identifier) {
        return static_cast<Port*>(new T(identifier));
    }
};


} // namespace

#endif // IVW_PORTFACTORYOBJECT_H

