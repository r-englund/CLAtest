#include <QGridLayout>

#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/qt/widgets/processors/canvasprocessorwidgetqt.h>

namespace inviwo {

CanvasProcessorWidgetQt::CanvasProcessorWidgetQt(Processor* processor, QWidget* parent)
    : ProcessorWidgetQt(processor, parent),
      canvas_(0)
{
    setMinimumSize(256, 256);
    setWindowFlags(windowFlags() | Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    setWindowTitle(QString::fromStdString(processor->getIdentifier())); 
}

CanvasProcessorWidgetQt::~CanvasProcessorWidgetQt() {
    if(canvas_) {
        canvas_->hide();
        ProcessorWidgetQt::hide();
        delete canvas_;
    }
}

void CanvasProcessorWidgetQt::initialize() {
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

void CanvasProcessorWidgetQt::show() {
    canvas_->show();
    ProcessorWidgetQt::show();
}

void CanvasProcessorWidgetQt::resizeEvent(QResizeEvent* event) {
    ProcessorWidgetQt::resizeEvent(event);
}

void CanvasProcessorWidgetQt::hide() {
    ProcessorWidgetQt::hide();
}

void CanvasProcessorWidgetQt::closeEvent(QCloseEvent *e) {    
    canvas_->hide();
    ProcessorWidgetQt::hide();
}

} // namespace
