#include <inviwo/qt/editor/mappingwidget.h>

namespace inviwo {

MappingWidget::MappingWidget(QWidget* parent) : InviwoDockWidget(tr("Mapping"), parent), VoidObserver() {
    setObjectName("MappingWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    inviwoApp_ = InviwoApplication::getPtr();    

    addObservation(inviwoApp_->getProcessorNetwork());
    inviwoApp_->getProcessorNetwork()->addObserver(this);             

    QFrame* frame_ = new QFrame();    
    QVBoxLayout* vLayout_ = new QVBoxLayout();
    label_ = new QLabel();
    
    vLayout_->addWidget(label_);
    frame_->setLayout(vLayout_);
    setWidget(frame_);

    updateWidget();
}

MappingWidget::~MappingWidget() {}

void MappingWidget::notify() {
    updateWidget();      
}

void MappingWidget::updateWidget() {
    std::vector<Processor*> curProcessorList = inviwoApp_->getProcessorNetwork()->getProcessors();
    label_->setText("Size of processor network: " + intToQString(curProcessorList.size()));
}

// For testing
QString MappingWidget::intToQString(int num ) {
    std::stringstream convert;
    convert << num;
    return convert.str().c_str();
}

} // namespace