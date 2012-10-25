#include "inviwo/qt/widgets/processors/processorwidgetqt.h"

namespace inviwo {

ProcessorWidgetQt::ProcessorWidgetQt(Processor* processor, QWidget* parent)
    : ProcessorWidget(processor)
{}

ProcessorWidgetQt::~ProcessorWidgetQt() {}

void ProcessorWidgetQt::show() {
    ProcessorWidget::show();
    QWidget::show();
}

void ProcessorWidgetQt::hide() {
    ProcessorWidget::hide();
    QWidget::hide();
}

void ProcessorWidgetQt::resizeEvent(QResizeEvent* event) {
    ProcessorWidget::hide();
    QWidget::resizeEvent(event);
}

} // namespace
