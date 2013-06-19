#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>

namespace inviwo {

inviwo::EventPropertyWidgetQt::EventPropertyWidgetQt( EventProperty* eventproperty ) {
    eventproperty_ = eventproperty;
    generateWidget();
}

void inviwo::EventPropertyWidgetQt::generateWidget() {
    delete layout(); // Make sure there is no previous layout
	std::string modifierName = eventproperty_->getEvent()->modifierName();
	if (modifierName != "") modifierName.append("-");

    std::string eventName = modifierName + eventproperty_->getEvent()->buttonName();
    std::string actionName = eventproperty_->getAction()->name();

    QHBoxLayout* hLayout = new QHBoxLayout();
    button_ = new QPushButton(eventName.c_str());
    QLabel* label_ = new QLabel(actionName.c_str());    

    QObject::connect(button_, SIGNAL(clicked()), this, SLOT(clickedSlot()));

    hLayout->addWidget(label_);
    hLayout->addWidget(button_);
    setLayout(hLayout);
}

void EventPropertyWidgetQt::updateFromProperty() {
    generateWidget();
}

void EventPropertyWidgetQt::clickedSlot() {
    MappingPopup* popup_ = new MappingPopup(eventproperty_, eventPropertyManager_);        
    popup_->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup );
    popup_->setFixedSize(1,1); // I am so sorry //FIX ME: Do it better
    popup_->show();

    button_->setText("PRESS A BUTTON");
    button_->setEnabled(false);
}

} //namespace

