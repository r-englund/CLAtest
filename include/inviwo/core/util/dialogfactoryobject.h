#ifndef IVW_DIALOGFACTORYOBJECT_H
#define IVW_DIALOGFACTORYOBJECT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API DialogFactoryObject { 
public:
    DialogFactoryObject(const std::string& className);
    virtual ~DialogFactoryObject(){}

    virtual Dialog* create() = 0;

    std::string getClassName() const;

private:
    std::string className_;
};

template<typename T>
class DialogFactoryObjectTemplate : public DialogFactoryObject {
public:
    DialogFactoryObjectTemplate(const std::string& className)
        : DialogFactoryObject(className) {}

    virtual ~DialogFactoryObjectTemplate() {}

    virtual Dialog* create() {
        return static_cast<Dialog*>(new T());
    }
};

} // namespace

#endif // IVW_DIALOGFACTORYOBJECT_H

