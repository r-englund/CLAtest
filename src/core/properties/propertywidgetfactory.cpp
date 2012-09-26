#include "inviwo/core/inviwoapplication.h"
#include "inviwo/core/inviwomodule.h"
#include "inviwo/core/properties/propertywidgetfactory.h"


namespace inviwo {

PropertyWidgetFactory* PropertyWidgetFactory::factory_ = 0;

PropertyWidgetFactory::PropertyWidgetFactory() {
    factory_ = this;
}

PropertyWidgetFactory::~PropertyWidgetFactory() {}

void PropertyWidgetFactory::initialize() {
    //TODO: check that inviwoapp is initialized

    /*
    InviwoApplication* inviwoApp = InviwoApplication::app();
    for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
        std::vector<Processor*> curProcessorList = inviwoApp->getModules()[curModuleId]->getProcessors();
        for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++)
            registerProcessor(curProcessorList[curProcessorId]);
    }
    */
}

void PropertyWidgetFactory::registerPropertyWidget(PropertyWidget* propertyWidget) {
    /*
    if (processorClassMap_.find(processor->getClassName()) == processorClassMap_.end())
        processorClassMap_.insert(std::make_pair(processor->getClassName(), processor));
    */
}

IvwSerializable* PropertyWidgetFactory::create(std::string className) const {
    /*
    std::map<std::string, PropertyWidget*>::iterator it = propertyWidgetMap_.find(className);
    if (it != propertyWidgetMap_.end())
        return it->second->create();
    else
    */
        return 0;

}

void PropertyWidgetFactory::deinitialize() {
}

PropertyWidgetFactory* PropertyWidgetFactory::instance() {
    return factory_;
}

} // namespace
