#include <QGridLayout>

#include "inviwo/core/processors/canvasprocessor.h"
#include "inviwo/qt/widgets/processors/canvasprocessorwidget.h"

namespace inviwo {

CanvasProcessorWidget::CanvasProcessorWidget(Processor* processor, QWidget* parent)
    : ProcessorWidgetQt(processor, parent),
      canvas_(0)
{
    setMinimumSize(256, 256);
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    setWindowTitle(QString::fromStdString(processor->getIdentifier())); 
}

CanvasProcessorWidget::~CanvasProcessorWidget() {
    if(canvas_) {
        canvas_->hide();
        ProcessorWidgetQt::hide();
        delete canvas_;
    }
}

void CanvasProcessorWidget::initialize() {
    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor_);
    canvas_ = new CanvasQt(this);
    canvas_->initialize();
    canvas_->setMouseTracking(true);
    
    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(dynamic_cast<QWidget*>(canvas_), 0, 0);
    setLayout(gridLayout);

    canvasProcessor->setCanvas(dynamic_cast<Canvas*>(canvas_));

    initialized_ = true;
}

void CanvasProcessorWidget::show() {
    canvas_->show();
    ProcessorWidgetQt::show();
}

void CanvasProcessorWidget::resizeEvent(QResizeEvent* event) {
    ProcessorWidgetQt::resizeEvent(event);
}

void CanvasProcessorWidget::hide() {
    ProcessorWidgetQt::hide();
}

void CanvasProcessorWidget::closeEvent(QCloseEvent *e) {    
    canvas_->hide();
    ProcessorWidgetQt::hide();
}

} // namespace
