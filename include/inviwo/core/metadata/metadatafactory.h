#ifndef IVW_METADATA_FACTORY_H
#define IVW_METADATA_FACTORY_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/metadata/metadata.h"
#include "inviwo/core/util/inviwofactorybase.h"
#include "inviwo/core/util/singleton.h"

namespace inviwo {

class IVW_CORE_API MetaDataFactory : public Factory,
                                     public Singleton<MetaDataFactory>  {

public:
    MetaDataFactory();
    virtual ~MetaDataFactory();

    virtual void initialize();
    virtual void deinitialize();

    void registerMetaData(MetaData* meta);
    virtual IvwSerializable* create(std::string className) const;
    virtual bool isValidType(std::string className) const;

private:
    mutable std::map<std::string, MetaData*> metaDataClassMap_;
};

} // namespace

#endif // IVW_PROCESSORFACTORY_H
