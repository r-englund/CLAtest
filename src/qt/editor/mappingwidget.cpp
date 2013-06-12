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
	QScrollArea* scrollArea = new QScrollArea();
    frame_->setLayout(vLayout_);
	scrollArea->setWidgetResizable(true);
	scrollArea->setWidget(frame_);
    setWidget(scrollArea);

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

	std::vector<EventProperty*> eventProperties, tmp;
	std::vector<InteractionHandler*> interactionHandlers;
	PropertyOwner* eventPropertyOwner;

	for (size_t i = 0; i < curProcessorList_.size(); ++i) {
		if (curProcessorList_.at(i)->hasInteractionHandler()) {			
			interactionHandlers = curProcessorList_.at(i)->getInteractionHandlers();
			for (size_t j = 0; j < interactionHandlers.size(); ++j) {
				eventPropertyOwner = dynamic_cast<PropertyOwner*>(interactionHandlers.at(j));
				if (eventPropertyOwner) /* Check if interactionhandlar has properties */{
					tmp = eventPropertyOwner->getPropertiesByType<EventProperty>();
					eventProperties.insert(eventProperties.end(), tmp.begin(), tmp.end());
				}
			}			
		}
	}	
	
	eventPropertyManager_->setEventProperties(eventProperties);
	drawEventPropertyWidgets(); 
}

// Draw the widgets for all EventProperties in EventPropertyManager
void MappingWidget::drawEventPropertyWidgets() {
	std::vector<EventProperty*> properties = eventPropertyManager_->getEventProperties();
	for (size_t i=0; i<properties.size(); i++) {
		EventProperty* curProperty = properties[i];
		PropertyWidgetQt* propertyWidget = PropertyWidgetFactoryQt::getRef().create(curProperty);
		vLayout_->addWidget(propertyWidget);
		curProperty->registerPropertyWidget(propertyWidget);
		dynamic_cast<EventPropertyWidgetQt*>(propertyWidget)->setManager(eventPropertyManager_);
	}
}

// Clears the layout of all widgets
void MappingWidget::emptyLayout() {
	while(!vLayout_->layout()->isEmpty()) {
		QWidget* w =  vLayout_->layout()->takeAt(0)->widget();
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