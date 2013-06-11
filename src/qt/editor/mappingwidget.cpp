#include <inviwo/qt/editor/mappingwidget.h>

namespace inviwo {

MappingWidget::MappingWidget(QWidget* parent) : InviwoDockWidget(tr("Mapping"), parent), VoidObserver() {
    setObjectName("MappingWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);   
	processorNetwork_ = InviwoApplication::getPtr()->getProcessorNetwork();
	eventPropertyManager_ = new EventPropertyManager();

    addObservation(processorNetwork_);
    processorNetwork_->addObserver(this);             
	
    frame_ = new QFrame();    
    vLayout_ = new QVBoxLayout();
	QScrollArea* scrollArea_ = new QScrollArea();
    frame_->setLayout(vLayout_);
	scrollArea_->setWidgetResizable(true);
	scrollArea_->setWidget(frame_);
    setWidget(scrollArea_);

    updateWidget();
}

MappingWidget::~MappingWidget() {}

void MappingWidget::notify() {
    updateWidget();      
}

void MappingWidget::updateWidget() {
	if (!vLayout_->isEmpty()) emptyLayout(); // Clear layout of widgets
    
	curProcessorList_ = processorNetwork_->getProcessors();
	label_ = new QLabel();
    label_->setText("Size of processor network: " + intToQString(curProcessorList_.size()));
	vLayout_->addWidget(label_);

	std::vector<EventProperty*> eventProperties_, tmp;
	std::vector<InteractionHandler*> interactionHandlers_;
	PropertyOwner* eventPropertyOwner_;

	for (size_t i = 0; i < curProcessorList_.size(); ++i) {
		if (curProcessorList_.at(i)->hasInteractionHandler()) {			
			interactionHandlers_ = curProcessorList_.at(i)->getInteractionHandlers();
			for (size_t j = 0; j < interactionHandlers_.size(); ++j) {
				eventPropertyOwner_ = dynamic_cast<PropertyOwner*>(interactionHandlers_.at(j));
				if (eventPropertyOwner_) /* Check if interactionhandlar has properties */{
					tmp = eventPropertyOwner_->getPropertiesByType<EventProperty>();
					eventProperties_.insert(eventProperties_.end(), tmp.begin(), tmp.end());
				}
			}			
		}
	}
	eventPropertyManager_->setEventProperties(eventProperties_);
	drawEventPropertyWidgets(); 
}

// Draw the widgets for all EventProperties in EventPropertyManager
void MappingWidget::drawEventPropertyWidgets() {
	std::vector<EventProperty*> properties = eventPropertyManager_->getEventProperties();
	for (size_t i=0; i<properties.size(); i++) {
		Property* curProperty = properties[i];
		PropertyWidgetQt* propertyWidget = PropertyWidgetFactoryQt::getRef().create(curProperty);
		vLayout_->addWidget(propertyWidget);
		curProperty->registerPropertyWidget(propertyWidget);
	}
}

// Clears the layout of all widgets
void MappingWidget::emptyLayout() {
	while(!vLayout_->layout()->isEmpty()) {
		QWidget *w =  vLayout_->layout()->takeAt(0)->widget();
		vLayout_->layout()->removeWidget(w);
		delete w;
	}
}

// For testing
QString MappingWidget::intToQString(int num ) {
    std::stringstream convert;
    convert << num;
    return convert.str().c_str();
}

} // namespace