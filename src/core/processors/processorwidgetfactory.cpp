#include <inviwo/core/inviwoapplication.h>
#include <inviwo/core/inviwomodule.h>
#include <inviwo/core/processors/processorwidgetfactory.h>


namespace inviwo {

ProcessorWidgetFactory::ProcessorWidgetFactory() {}

ProcessorWidgetFactory::~ProcessorWidgetFactory() {}

void ProcessorWidgetFactory::initialize() {
}

void ProcessorWidgetFactory::registerProcessorWidget(ProcessorWidget* processorWidget) {
}

IvwSerializable* ProcessorWidgetFactory::create(std::string className) const {
        return 0;
}

bool ProcessorWidgetFactory::isValidType(std::string className) const {
    return false;
}

void ProcessorWidgetFactory::deinitialize() {
}

} // namespace
