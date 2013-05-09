#include <inviwo/core/processors/processorwidget.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

ProcessorWidget::ProcessorWidget(Processor* processor)
    : processor_(processor)
{
    initialized_ = false;
    metaData_ = dynamic_cast<ProcessorWidgetMetaData*>(processor_->getMetaData("ProcessorWidgetMetaData"));
}

ProcessorWidget::~ProcessorWidget() {}

void ProcessorWidget::initialize() {}

void ProcessorWidget::show() {
    metaData_->setVisibile(true);
}

void ProcessorWidget::hide() {    
    metaData_->setVisibile(false);
}

void ProcessorWidget::setDimensions(ivec2 dimension) {   
    metaData_->setDimension(dimension);
}

void ProcessorWidget::move(ivec2 pos) {    
    metaData_->setWidgetPosition(pos);
}

bool ProcessorWidget::getVisibilityMetaData() {    
    return metaData_->isVisible();
}

ivec2 ProcessorWidget::getPositionMetaData() {
    return metaData_->getWidgetPosition();
}

ivec2 ProcessorWidget::getDimensionMetaData() {
    return metaData_->getDimension();
}

} // namespace
