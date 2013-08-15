#include <inviwo/qt/editor/eventpropertymanagerwidget.h>
#include <inviwo/qt/widgets/properties/collapsivegroupboxwidgetqt.h>

namespace inviwo {	

EventPropertyManagerWidget::EventPropertyManagerWidget(EventPropertyManager* eventPropertyManager) : VoidObserver() {
	eventPropertyManager_ = eventPropertyManager;

	addObservation(eventPropertyManager_);
	eventPropertyManager_->addObserver(this);

	groupCollapsed["Step rotation"] = true;
	groupCollapsed["Step zooming"] = true;
	groupCollapsed["Step panning"] = true;

	mainLayout_ = new QVBoxLayout();
	mainLayout_->setAlignment(Qt::AlignTop);
	mainLayout_->setSpacing(0);
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
		CollapsiveGroupBoxWidgetQt* box = dynamic_cast<CollapsiveGroupBoxWidgetQt*>(w);
		if (box)
			groupCollapsed[box->getIdentifier()] = box->isCollapsed();

		layout->layout()->removeWidget(w);
		delete w;
	}
}

// Draw the widgets for all EventProperties in EventPropertyManager
void EventPropertyManagerWidget::drawEventPropertyWidgets() {
	std::vector<EventProperty*> properties = eventPropertyManager_->getEventPropertiesFromMap();
	std::vector<Property*> addedProperties;

	for (size_t i = 0; i < properties.size(); ++i) {
		if (std::find(addedProperties.begin(), addedProperties.end(), properties[i]) != addedProperties.end())
			continue;

		if (properties[i]->getGroupID()!="") {
			CollapsiveGroupBoxWidgetQt* group = new CollapsiveGroupBoxWidgetQt(properties[i]->getGroupID());
			//Add all the properties with the same group assigned
			for (size_t k=0; k<properties.size(); k++){
				Property* tmpProperty = properties[k];
				if (properties[i]->getGroupID() == tmpProperty->getGroupID()) {
					group->addProperty(tmpProperty);
					addedProperties.push_back(tmpProperty);					
				}
			}
			group->generateEventPropertyWidgets(eventPropertyManager_);
			if (groupCollapsed[group->getIdentifier()])
				group->hide();

			mainLayout_->addWidget(group);
		} else {
			PropertyWidgetQt* propertyWidget = PropertyWidgetFactoryQt::getRef().create(properties[i]);
			mainLayout_->addWidget(propertyWidget);
			properties[i]->registerPropertyWidget(propertyWidget);
			dynamic_cast<EventPropertyWidgetQt*>(propertyWidget)->setManager(eventPropertyManager_);
		}
	}
}

} //inviwo