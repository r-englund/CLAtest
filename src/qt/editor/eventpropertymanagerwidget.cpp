#include <inviwo/qt/editor/eventpropertymanagerwidget.h>

namespace inviwo {	

EventPropertyManagerWidget::EventPropertyManagerWidget(EventPropertyManager* eventPropertyManager) : VoidObserver() {
	eventPropertyManager_ = eventPropertyManager;

	addObservation(eventPropertyManager_);
	eventPropertyManager_->addObserver(this);

	mainLayout_ = new QVBoxLayout();
	setLayout(mainLayout_);
}

EventPropertyManagerWidget::~EventPropertyManagerWidget(){}

void EventPropertyManagerWidget::notify() {
	emptyLayout(mainLayout_);
	if (!eventPropertyManager_->isEmpty()) {
		drawEventPropertyWidgets();
	}
}

void EventPropertyManagerWidget::emptyLayout(QVBoxLayout* layout) {
	while(!layout->layout()->isEmpty()) {
		QWidget* w =  layout->layout()->takeAt(0)->widget();
		layout->layout()->removeWidget(w);
		delete w;
	}
}

// Draw the widgets for all EventProperties in EventPropertyManager
void EventPropertyManagerWidget::drawEventPropertyWidgets() {
	std::vector<EventProperty*> properties = eventPropertyManager_->getEventPropertiesFromMap();

	for (size_t i=0; i<properties.size(); i++) {
		EventProperty* curProperty = properties[i];
		PropertyWidgetQt* propertyWidget = PropertyWidgetFactoryQt::getRef().create(curProperty);
		mainLayout_->addWidget(propertyWidget);
		curProperty->registerPropertyWidget(propertyWidget);
		dynamic_cast<EventPropertyWidgetQt*>(propertyWidget)->setManager(eventPropertyManager_);
	}
}

} //inviwo