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
 * Main file author: Hans-Christian Helltegen
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/eventpropertymanagerwidget.h>
#include <inviwo/qt/widgets/properties/collapsiblegroupboxwidgetqt.h>
#include <inviwo/core/properties/propertywidgetfactory.h>

namespace inviwo {

EventPropertyManagerWidget::EventPropertyManagerWidget(EventPropertyManager* eventPropertyManager) : EventPropertyManagerObserver() {
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

EventPropertyManagerWidget::~EventPropertyManagerWidget() {}

void EventPropertyManagerWidget::onEventPropertyManagerChange() {
    emptyLayout(mainLayout_);

    if (!eventPropertyManager_->isEmpty())
        drawEventPropertyWidgets();
}

void EventPropertyManagerWidget::emptyLayout(QVBoxLayout* layout) {
    while (!layout->isEmpty()) {
        QWidget* w =  layout->takeAt(0)->widget();

        layout->removeWidget(w);
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


        PropertyWidgetQt* propertyWidget =
            static_cast<PropertyWidgetQt*>(PropertyWidgetFactory::getPtr()->create(properties[i]));
        mainLayout_->addWidget(propertyWidget);
        properties[i]->registerWidget(propertyWidget);
        dynamic_cast<EventPropertyWidgetQt*>(propertyWidget)->setManager(eventPropertyManager_);
    }
}

} //inviwo