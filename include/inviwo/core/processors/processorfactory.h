#ifndef IVW_PROCESSORFACTORY_H
#define IVW_PROCESSORFACTORY_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/processors/processor.h"
#include "inviwo/core/inviwofactorybase.h"

namespace inviwo {

    class IVW_CORE_API ProcessorFactory : public InviwoFactoryBase {

    public:
        ProcessorFactory();
        virtual ~ProcessorFactory();

        static ProcessorFactory* instance();

        virtual void initialize();
        virtual void deinitialize();

        void registerProcessor(Processor* processor);
        virtual IvwSerializable* create(std::string className) const;
        virtual bool isValidType(std::string className) const;

    protected:
        static ProcessorFactory* factory_;

    private:
        mutable std::map<std::string, Processor*> processorClassMap_;
    };

} // namespace

#endif // IVW_PROCESSORFACTORY_H
