#include <inviwo/qt/editor/mappingwidget.h>

namespace inviwo {

MappingWidget::MappingWidget(QWidget* parent) : InviwoDockWidget(tr("Mapping"), parent), VoidObserver() {
    setObjectName("MappingWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);   
	processorNetwork_ = InviwoApplication::getPtr()->getProcessorNetwork();
	eventPropertyManager_ = new EventPropertyManager();

    addObservation(processorNetwork_);
    processorNetwork_->addObserver(this);             
	
	buildLayout();	
	currentIndex_ = 0;

	processorsWithInteractionHandlers_ = findProcessorsWithInteractionHandlers(processorNetwork_->getProcessors());
    updateWidget();
}

MappingWidget::~MappingWidget() {}

void MappingWidget::buildLayout() {
	// Components needed for layout
	QFrame* frame_ = new QFrame();
	botLayout_ = new QVBoxLayout();
	comboBox_ = new QComboBox();
	mainLayout = new QVBoxLayout();
	QVBoxLayout* topLayout = new QVBoxLayout();
	QScrollArea* scrollArea = new QScrollArea();
	QWidget* area = new QWidget();

	// mainLayout contains topLayout and scrollArea.
	// scrollArea contains a widget with the botLayout.
	// Eventpropertywidgets will be added to the botlayout.
	topLayout->addWidget(comboBox_);
	area->setLayout(botLayout_);
	scrollArea->setWidget(area);
	scrollArea->setWidgetResizable(true);
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(scrollArea);
	frame_->setLayout(mainLayout);
	setWidget(frame_);
	
	QObject::connect(comboBox_, SIGNAL(activated(int)), this, SLOT(comboBoxChange()));
}

void MappingWidget::notify() {
	processorsWithInteractionHandlers_ = findProcessorsWithInteractionHandlers(processorNetwork_->getProcessors());
	if (processorsWithInteractionHandlers_.size() != prevProcessorsWithInteractionHandlers_.size()) {
		updateWidget();
	} 
}

void MappingWidget::updateWidget() {
	if (comboBox_->count() > 0)	{
		currentIndex_ =  comboBox_->currentIndex();
	}

	emptyLayout(mainLayout); // Clear layout of widgets
	buildLayout();

	std::vector<EventProperty*> eventProperties, tmp;
	std::vector<InteractionHandler*> interactionHandlers;
	std::map<std::string, std::vector<EventProperty*> > eventPropertyMap;
	PropertyOwner* eventPropertyOwner;

	// Get all eventproperties from the processornetwork
	for (size_t i = 0; i < processorsWithInteractionHandlers_.size(); ++i) {	
		interactionHandlers = processorsWithInteractionHandlers_.at(i)->getInteractionHandlers();
		for (size_t j = 0; j < interactionHandlers.size(); ++j) {
			eventPropertyOwner = dynamic_cast<PropertyOwner*>(interactionHandlers.at(j));
			if (eventPropertyOwner) /* Check if interactionhandlar has properties */{
				tmp = eventPropertyOwner->getPropertiesByType<EventProperty>();
				eventProperties.insert(eventProperties.end(), tmp.begin(), tmp.end());
			}
		}
		// Add vector of eventproperties to map with processor identifier as key
		eventPropertyMap[processorsWithInteractionHandlers_.at(i)->getIdentifier()] = eventProperties;
		comboBox_->addItem(processorsWithInteractionHandlers_.at(i)->getIdentifier().c_str());
		eventProperties.clear();		
	}

	if (currentIndex_ > comboBox_->count()-1) currentIndex_ = 0;
	
	eventPropertyManager_->setEventPropertyMap(eventPropertyMap);

	// Draw eventpropertywidgets
	if (comboBox_->count() > 0) {
		comboBox_->setCurrentIndex(currentIndex_);
		drawEventPropertyWidgets(); 
	}

	prevProcessorsWithInteractionHandlers_ = processorsWithInteractionHandlers_;
}

// Draw the widgets for all EventProperties in EventPropertyManager
void MappingWidget::drawEventPropertyWidgets() {	
	std::string identifier = (const char *)comboBox_->currentText().toLatin1();
	eventPropertyManager_->setActiveProcessor(identifier.c_str());
	std::vector<EventProperty*> properties = eventPropertyManager_->getEventPropertiesFromMap();
	
	for (size_t i=0; i<properties.size(); i++) {
		EventProperty* curProperty = properties[i];
		PropertyWidgetQt* propertyWidget = PropertyWidgetFactoryQt::getRef().create(curProperty);
		botLayout_->addWidget(propertyWidget);
		curProperty->registerPropertyWidget(propertyWidget);
		dynamic_cast<EventPropertyWidgetQt*>(propertyWidget)->setManager(eventPropertyManager_);
	}
}

// Clears the layout of all widgets
void MappingWidget::emptyLayout(QVBoxLayout* layout) {
	QObject::disconnect(this, SLOT(comboBoxChange()));
	while(!layout->layout()->isEmpty()) {
		QWidget* w =  layout->layout()->takeAt(0)->widget();
		layout->layout()->removeWidget(w);
		delete w;
	}
}

void MappingWidget::comboBoxChange() {
	emptyLayout(botLayout_);
	drawEventPropertyWidgets();
}

std::vector<Processor*> MappingWidget::findProcessorsWithInteractionHandlers(std::vector<Processor*> processors) {
	std::vector<Processor*> processorsWithInteractionHandlers;
	for (size_t i = 0; i < processors.size(); ++i) {
		if (processors.at(i)->hasInteractionHandler()) {
			processorsWithInteractionHandlers.push_back(processors.at(i));
		}
	}
	return processorsWithInteractionHandlers;
}

} // namespace