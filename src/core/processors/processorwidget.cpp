#include "inviwo/core/processors/processorwidget.h"

namespace inviwo {

ProcessorWidget::ProcessorWidget(Processor* processor)
    : processor_(processor)
{
    initialized_ = false;
}

ProcessorWidget::~ProcessorWidget() {}

void ProcessorWidget::initialize() {}

void ProcessorWidget::show() {
    //std::cout << "showing" << std::endl;
}

void ProcessorWidget::hide() {
    //std::cout << "hiding" << std::endl;
}

} // namespace
