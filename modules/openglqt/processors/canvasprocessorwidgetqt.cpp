#include <QGridLayout>

#include <inviwo/core/processors/canvasprocessor.h>
#include <modules/openglqt/processors/canvasprocessorwidgetqt.h>

namespace inviwo {

CanvasProcessorWidgetQt::CanvasProcessorWidgetQt()
    : ProcessorWidgetQt(),
      canvas_(0)
{
    setMinimumSize(32, 32);
    setWindowFlags(windowFlags() | Qt::Tool | Qt::CustomizeWindowHint);
    setWindowTitle(QString::fromStdString("untitled canvas")); 
}

CanvasProcessorWidgetQt::~CanvasProcessorWidgetQt() {
    if (canvas_) {
        canvas_->hide();
        ProcessorWidgetQt::hide();
        delete canvas_;
    }
}

ProcessorWidget* CanvasProcessorWidgetQt::create() const {
    return new CanvasProcessorWidgetQt();
}

void CanvasProcessorWidgetQt::initialize() {    
    setWindowTitle(QString::fromStdString(processor_->getIdentifier())); 
    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor_);
    canvas_ = new CanvasQt(this);
    canvas_->initialize();
    canvas_->setMouseTracking(true);
    
    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(static_cast<QWidget*>(canvas_), 0, 0);
    setLayout(gridLayout);

    canvasProcessor->setCanvas(static_cast<Canvas*>(canvas_));    
    uvec2 csize = canvasProcessor->getCanvas()->size();
    resize(static_cast<int>(csize[0]), static_cast<int>(csize[1]));
    
    ProcessorWidgetQt::initialize();
    initialized_ = true;
}

void CanvasProcessorWidgetQt::resizeEvent(QResizeEvent* event) {
    ProcessorWidgetQt::resizeEvent(event);
}

void CanvasProcessorWidgetQt::show() {
    canvas_->show();
    ProcessorWidgetQt::show();
}

void CanvasProcessorWidgetQt::showEvent(QShowEvent* event) {
    canvas_->show();
    canvas_->activate();
    canvas_->update();
    ProcessorWidgetQt::showEvent(event);
}

void CanvasProcessorWidgetQt::hide() {
    canvas_->hide();
    ProcessorWidgetQt::hide();
}

void CanvasProcessorWidgetQt::closeEvent(QCloseEvent *event) {    
    canvas_->hide();
    ProcessorWidgetQt::closeEvent(event);
}

} // namespace
