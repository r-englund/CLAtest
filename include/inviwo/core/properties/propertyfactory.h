#ifndef IVW_PROCESSORFACTORY_H
#define IVW_PROCESSORFACTORY_H

#include "inviwo/core/properties/property.h"
#include "inviwo/core/inviwofactorybase.h"

namespace inviwo {

    class PropertyFactory : public InviwoFactoryBase {

    public:
        PropertyFactory();
        ~PropertyFactory();

        static PropertyFactory* instance();

        virtual void initialize();
        virtual void deinitialize();

        void registerProcessor(Property* property);
        virtual IvwSerializable* PropertyFactory::create(std::string className) const;

    protected:
        static PropertyFactory* factory_;

    private:
        mutable std::map<std::string, Property*> processorClassMap_;
    };

} // namespace

#endif // IVW_PROCESSORFACTORY_H
