#ifndef IVW_PROCESSORFACTORY_H
#define IVW_PROCESSORFACTORY_H

#include "inviwo/core/processors/processor.h"

namespace inviwo {

    class ProcessorFactory {

    public:
        ProcessorFactory();
        ~ProcessorFactory();

        static ProcessorFactory* instance();

        void initialize();
        void deinitialize();

        void registerProcessor(Processor* processor);
        Processor* ProcessorFactory::create(std::string className) const;

    protected:
        static ProcessorFactory* factory_;

    private:
        mutable std::map<std::string, Processor*> processorClassMap_;
    };

} // namespace

#endif // IVW_PROCESSORFACTORY_H
