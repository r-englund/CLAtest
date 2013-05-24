#ifndef IVW_PROCESSORFACTORY_H
#define IVW_PROCESSORFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/processors/processorfactoryobject.h>
#include <inviwo/core/util/inviwofactorybase.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_CORE_API ProcessorFactory : public Factory,
                                      public Singleton<ProcessorFactory>  {

public:
    ProcessorFactory();
    virtual ~ProcessorFactory();

    virtual void initialize();
    virtual void deinitialize();

    void registerFactoryObject(ProcessorFactoryObject* processor);
    virtual IvwSerializable* create(std::string className) const;
    virtual bool isValidType(std::string className) const;

private:
    mutable std::map<std::string, ProcessorFactoryObject*> processorClassMap_;
};

} // namespace

#endif // IVW_PROCESSORFACTORY_H
