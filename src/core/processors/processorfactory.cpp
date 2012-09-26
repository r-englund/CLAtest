#include "inviwo/core/inviwoapplication.h"
#include "inviwo/core/inviwomodule.h"
#include "inviwo/core/processors/processorfactory.h"
#include "inviwo/core/io/serialization/ivwserializable.h"


namespace inviwo {

ProcessorFactory* ProcessorFactory::factory_ = 0;

ProcessorFactory::ProcessorFactory() {
    factory_ = this;
}

ProcessorFactory::~ProcessorFactory() {}

void ProcessorFactory::initialize() {
    //TODO: check that inviwoapp is initialized

    InviwoApplication* inviwoApp = InviwoApplication::app();
    for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
        std::vector<Processor*> curProcessorList = inviwoApp->getModules()[curModuleId]->getProcessors();
        for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++)
            registerProcessor(curProcessorList[curProcessorId]);
    }
}

void ProcessorFactory::registerProcessor(Processor* processor) {
    if (processorClassMap_.find(processor->getClassName()) == processorClassMap_.end())
        processorClassMap_.insert(std::make_pair(processor->getClassName(), processor));
}

IvwSerializable* ProcessorFactory::create(std::string className) const {
    std::map<std::string, Processor*>::iterator it = processorClassMap_.find(className);
    if (it != processorClassMap_.end())
        return it->second->create();
    else
        return 0;
}

void ProcessorFactory::deinitialize() {
}

ProcessorFactory* ProcessorFactory::instance() {
    return factory_;
}

} // namespace
