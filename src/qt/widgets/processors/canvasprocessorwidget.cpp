#include <QGridLayout>

#include "inviwo/core/processors/canvasprocessor.h"
#include "inviwo/qt/widgets/processors/canvasprocessorwidget.h"

namespace inviwo {

    CanvasProcessorWidget::CanvasProcessorWidget(Processor* processor, QWidget* parent)
        : ProcessorWidgetQt(processor, parent),
          canvas_(0)
    {
        setMinimumSize(256, 256); 
    }

    CanvasProcessorWidget::~CanvasProcessorWidget() {}

    void CanvasProcessorWidget::initialize() {
        CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor_);
        canvas_ = new CanvasQt(this);

        QGridLayout* layout = new QGridLayout;
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(dynamic_cast<QWidget*>(canvas_), 0, 0);
        setLayout(layout);


        canvasProcessor->setCanvas(dynamic_cast<Canvas*>(canvas_));
        initialized_ = true;
    }

    void CanvasProcessorWidget::show() {
        //std::cout << "showing" << std::endl;
        ProcessorWidgetQt::show();
    }

    void CanvasProcessorWidget::hide() {
        //std::cout << "hiding" << std::endl;
        ProcessorWidgetQt::hide();
    }


} // namespace
