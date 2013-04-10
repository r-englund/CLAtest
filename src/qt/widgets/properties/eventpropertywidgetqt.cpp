#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>

namespace inviwo {

inviwo::EventPropertyWidgetQt::EventPropertyWidgetQt( EventProperty* eventproperty ) {
    eventproperty_ = eventproperty;
    generateWidget();
}


void inviwo::EventPropertyWidgetQt::generateWidget() {

    std::string eventName = eventproperty_->getEvent().modifierName() + eventproperty_->getEvent().buttonName();
    std::string actionName = eventproperty_->getAction().name();

    QHBoxLayout* hLayout = new QHBoxLayout();
    QPushButton* button_ = new QPushButton(eventName.c_str());
    QLabel* label_ = new QLabel(actionName.c_str());    

    hLayout->addWidget(label_);
    hLayout->addWidget(button_);
    setLayout(hLayout);
}


void EventPropertyWidgetQt::updateFromProperty() {

}

} //namespace

