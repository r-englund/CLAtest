#ifndef IVW_DIALOGFACTORY_H
#define IVW_DIALOGFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/util/factory.h>
#include <inviwo/core/util/singleton.h>
#include <inviwo/core/util/dialog.h>
#include <inviwo/core/util/dialogfactory.h>
#include <inviwo/core/util/dialogfactoryobject.h>

namespace inviwo {

class IVW_CORE_API DialogFactory : public Factory,
                                   public Singleton<DialogFactory> {
public:
    DialogFactory();
    virtual ~DialogFactory();

    virtual Dialog* getDialog(std::string className) const;

    void registerObject(DialogFactoryObject* dialog);
    virtual IvwSerializable* create(std::string className) const;
    virtual bool isValidType(std::string className) const;

    typedef std::multimap<std::string, DialogFactoryObject*> DialogMap;
private:
    mutable DialogMap map_;
};

} // namespace

#endif // IVW_DIALOGFACTORY_H

