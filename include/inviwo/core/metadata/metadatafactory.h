#ifndef IVW_METADATA_FACTORY_H
#define IVW_METADATA_FACTORY_H

#include "inviwo/core/metadata/metadata.h"
#include "inviwo/core/inviwofactorybase.h"

namespace inviwo {

    class MetaDataFactory : public InviwoFactoryBase {

    public:
        MetaDataFactory();
        virtual ~MetaDataFactory();

        static MetaDataFactory* instance();

        virtual void initialize();
        virtual void deinitialize();

        void registerMetaData(MetaData* meta);
        virtual IvwSerializable* create(std::string className) const;
        virtual bool isValidType(std::string className) const;

    protected:
        static MetaDataFactory* factory_;

    private:
        mutable std::map<std::string, MetaData*> metaDataClassMap_;
    };

} // namespace

#endif // IVW_PROCESSORFACTORY_H
