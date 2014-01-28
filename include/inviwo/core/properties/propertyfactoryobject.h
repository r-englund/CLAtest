#ifndef IVW_PROPERTYFACTORYOBJECT_H
#define IVW_PROPERTYFACTORYOBJECT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API PropertyFactoryObject { 
public:
    PropertyFactoryObject(const std::string& className);
    virtual ~PropertyFactoryObject();

    virtual Property* create(std::string identifier,
                             std::string displayName) = 0;

    std::string getClassName() const;

private:
    std::string className_;
};

template<typename T>
class PropertyFactoryObjectTemplate : public PropertyFactoryObject {
public:
    PropertyFactoryObjectTemplate(const std::string& className)
        : PropertyFactoryObject(className) {}

    virtual ~PropertyFactoryObjectTemplate() {}

    virtual Property* create(std::string identifier,
                             std::string displayName) {
        return static_cast<Property*>(new T(identifier, displayName));
    }
};

} // namespace

#endif // IVW_PROPERTYFACTORYOBJECT_H

