/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Contact: Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/eventconverterqt.h>
#include <inviwo/qt/widgets/mappingpopup.h>

#include <QPushButton>

namespace inviwo {

EventPropertyWidgetQt::EventPropertyWidgetQt(EventProperty* eventproperty)
    : PropertyWidgetQt(eventproperty) {
    eventproperty_ = eventproperty;
    generateWidget();
}

void inviwo::EventPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    button_ = new QPushButton();

    label_ = new EditableLabelQt(this, eventproperty_->getDisplayName(), false);
    connect(label_, SIGNAL(textChanged()), this, SLOT(setPropertyDisplayName()));

    connect(button_, SIGNAL(clicked()), this, SLOT(clickedSlot()));
    hLayout->addWidget(label_);
    hLayout->addWidget(button_);
    setLayout(hLayout);

    setButtonText();
}

void EventPropertyWidgetQt::updateFromProperty() {
    setButtonText();
}

void EventPropertyWidgetQt::clickedSlot() {
    grabKeyboard();

    tmpEvent.setButton(0);
    tmpEvent.setModifiers(0);

    button_->setText("Press a button");
    button_->setEnabled(false);
}

void EventPropertyWidgetQt::keyPressEvent(QKeyEvent * event) {
    LogInfo("Press: " << event->key() << " txt " << event->text().toStdString() << " m " << event->modifiers());
    
    if (event->key() != Qt::Key_Enter && event->key() != Qt::Key_Return && event->key() != Qt::Key_Escape) {

        int key = EventConverterQt::getKeyButton(event);
        int modifer = EventConverterQt::getModifier(event);
    
        tmpEvent.setButton(key);
        tmpEvent.setModifiers(tmpEvent.modifiers() | modifer);

        std::string text = tmpEvent.modifierNames();
        if (text != "") text.append("-");
        text += std::string(1, static_cast<char>(tmpEvent.button()));

        button_->setText(QString::fromStdString(text));
    }

    QWidget::keyPressEvent(event);
}

void EventPropertyWidgetQt::keyReleaseEvent(QKeyEvent * event) {
    LogInfo("Release: " << event->key() << " txt " << event->text().toStdString() << " m " << event->modifiers());

    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        releaseKeyboard();
        eventproperty_->setEvent(tmpEvent.clone());
        setButtonText();
        button_->setEnabled(true); 
    } else if (event->key() == Qt::Key_Escape) {
        releaseKeyboard();
        setButtonText();  
        button_->setEnabled(true);
    } else {
        QWidget::keyReleaseEvent(event);
    }
}

void EventPropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}

void EventPropertyWidgetQt::setButtonText() {
    std::string text = eventproperty_->getEvent()->modifierNames();

    if (text != "") text.append("-");


    KeyboardEvent* keyboardEvent = dynamic_cast<KeyboardEvent*>(eventproperty_->getEvent());
    if (keyboardEvent) { 
        text += std::string(1, static_cast<char>(keyboardEvent->button()));
    }

    MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(eventproperty_->getEvent());
    if (mouseEvent) {
        text += mouseEvent->buttonName();
    }

    button_->setText(QString::fromStdString(text));
}


} //namespace


