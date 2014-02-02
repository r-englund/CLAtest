/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Hans-Christian Helltegen
 *
 **********************************************************************/

#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>

namespace inviwo {

inviwo::EventPropertyWidgetQt::EventPropertyWidgetQt(EventProperty* eventproperty) {
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
    popup_->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    popup_->setFixedSize(1,1); // I am so sorry //FIX ME: Do it better
    popup_->show();
    button_->setText("PRESS A BUTTON");
    button_->setEnabled(false);
}

} //namespace

