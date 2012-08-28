#include <QGridLayout>

#include "inviwo/core/processors/canvasprocessor.h"
#include "inviwo/qt/widgets/processors/canvasprocessorwidget.h"

namespace inviwo {

CanvasProcessorWidget::CanvasProcessorWidget(Processor* processor, QWidget* parent)
    : ProcessorWidgetQt(processor, parent),
      canvas_(0)
{
    setMinimumSize(256, 256);
    setMaximumSize(256, 256);
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    setWindowTitle(QString::fromStdString(processor->getIdentifier()));
}

CanvasProcessorWidget::~CanvasProcessorWidget() {}

void CanvasProcessorWidget::initialize() {
    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor_);
    canvas_ = new CanvasQt(this);
    canvas_->initialize();
    canvasProcessor->setCanvas(dynamic_cast<Canvas*>(canvas_));

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(dynamic_cast<QWidget*>(canvas_), 0, 0);
    setLayout(gridLayout);

    initialized_ = true;
}

void CanvasProcessorWidget::show() {
    ProcessorWidgetQt::show();
}

void CanvasProcessorWidget::hide() {
    ProcessorWidgetQt::hide();
}

} // namespace
