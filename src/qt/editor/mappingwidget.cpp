#include <inviwo/qt/editor/mappingwidget.h>

namespace inviwo {

MappingWidget::MappingWidget(QWidget* parent) : InviwoDockWidget(tr("Mapping"), parent), VoidObserver() {
    setObjectName("MappingWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    inviwoApp_ = InviwoApplication::getPtr();
    addObservation(inviwoApp_->getProcessorNetwork());
    inviwoApp_->getProcessorNetwork()->addObserver(this);

    std::vector<Processor*> curProcessorList = inviwoApp_->getProcessorNetwork()->getProcessors();          
    /*
    std::cout << "---" << std::endl;
    for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++)
        std::cout << curProcessorList[curProcessorId]->getClassName() << std::endl;
    */

    QFrame* frame = new QFrame();
    label = new QLabel("Size of processornetwork: " + intToQString(curProcessorList.size()));
    QVBoxLayout* vLayout_ = new QVBoxLayout(frame);      
    
    vLayout_->addWidget(label);
    frame->setLayout(vLayout_);
    setWidget(frame);
}

MappingWidget::~MappingWidget() {}

void MappingWidget::notify() {
    std::vector<Processor*> curProcessorList = inviwoApp_->getProcessorNetwork()->getProcessors();
    label->setText("Size of processornetwork: " + intToQString(curProcessorList.size()));  
    /*
    std::cout << "---" << std::endl;
    for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++)
        std::cout << curProcessorList[curProcessorId]->getClassName() << std::endl;
    */
}

QString MappingWidget::intToQString(int num ) {
    std::stringstream convert;
    convert << num;
    return convert.str().c_str();
}

} // namespace