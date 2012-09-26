#include "inviwo/core/inviwoapplication.h"
#include "inviwo/core/inviwomodule.h"
#include "inviwo/core/properties/propertyfactory.h"


namespace inviwo {

    PropertyFactory* PropertyFactory::factory_ = 0;

    PropertyFactory::PropertyFactory() {
        factory_ = this;
    }

    PropertyFactory::~PropertyFactory() {}

    void PropertyFactory::initialize() {
        //TODO: check that inviwoapp is initialized

        /*InviwoApplication* inviwoApp = InviwoApplication::app();
        for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
            std::vector<Processor*> curProcessorList = inviwoApp->getModules()[curModuleId]->getProcessors();
            for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++)
                registerProcessor(curProcessorList[curProcessorId]);
        }*/
    }

    void PropertyFactory::registerProcessor(Property* property) {
       /* if (processorClassMap_.find(processor->getClassName()) == processorClassMap_.end())
            processorClassMap_.insert(std::make_pair(processor->getClassName(), processor));*/
    }

    IvwSerializable* PropertyFactory::create(std::string className) const {
        /*std::map<std::string, Processor*>::iterator it = processorClassMap_.find(className);
        if (it != processorClassMap_.end())
            return it->second->create();
        else*/
            return 0;
    }

    void PropertyFactory::deinitialize() {
    }

    PropertyFactory* PropertyFactory::instance() {
        return factory_;
    }

} // namespace
