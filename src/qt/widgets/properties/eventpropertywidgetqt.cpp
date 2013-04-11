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

    QObject::connect(button_, SIGNAL(clicked()), this, SLOT(clickedSlot()));

    hLayout->addWidget(label_);
    hLayout->addWidget(button_);
    setLayout(hLayout);
}


void EventPropertyWidgetQt::updateFromProperty() {

}

void EventPropertyWidgetQt::clickedSlot() {
    MappingPopup* popup_ = new MappingPopup(eventproperty_);
    std::string popupLabelText_ = "Press button to map to " + eventproperty_->getAction().name();
    QLabel* popupLabel_ = new QLabel(popupLabelText_.c_str());
    QHBoxLayout* popupLayout_ = new QHBoxLayout();

    popupLayout_->addWidget(popupLabel_);    
    popup_->setWindowFlags(Qt::WindowStaysOnTopHint);
    popup_->setWindowTitle("Map");
    popup_->setLayout(popupLayout_);
    popup_->show();
    
    
}

} //namespace

