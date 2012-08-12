#include "inviwo/qt/widgets/processors/processorwidgetqt.h"

namespace inviwo {

    ProcessorWidgetQt::ProcessorWidgetQt(Processor* processor, QWidget* parent)
        : ProcessorWidget(processor)
    {}

    ProcessorWidgetQt::~ProcessorWidgetQt() {}

    void ProcessorWidgetQt::show() {
        //std::cout << "showing" << std::endl;
        ProcessorWidget::show();
        QWidget::show();
    }

    void ProcessorWidgetQt::hide() {
        //std::cout << "hiding" << std::endl;
        ProcessorWidget::hide();
        QWidget::hide();
    }


} // namespace
