#ifndef IVW_PORTFACTORY_H
#define IVW_PORTFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/portfactoryobject.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_CORE_API PortFactory : public Factory,
    public Singleton<PortFactory> {
public:
    PortFactory();
    virtual ~PortFactory();

    void registeryObject(PortFactoryObject* property);

    virtual IvwSerializable* create(std::string className) const;

    virtual Port* getPort(std::string className,
                          std::string identifier);

    virtual bool isValidType(std::string className) const;

    std::vector<std::string> getRegistedPortClassNames();

    typedef std::map<std::string, PortFactoryObject*> PortClassMap;

private:
    mutable PortClassMap portClassMap_;
};

} // namespace

#endif // IVW_PORTFACTORY_H

