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
 * Main file authors: Hans-Christian Helltegen, Erik Sundén
 *
 *********************************************************************************/

#include <inviwo/qt/editor/mappingwidget.h>
#include <inviwo/core/network/processornetwork.h>
#include <inviwo/qt/widgets/eventpropertymanager.h>
#include <inviwo/qt/widgets/eventpropertymanagerwidget.h>
#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

MappingWidget::MappingWidget(QWidget* parent) : InviwoDockWidget(tr("Input Mapping"), parent), ProcessorNetworkObserver() {
    setObjectName("MappingWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    processorNetwork_ = InviwoApplication::getPtr()->getProcessorNetwork();
    eventPropertyManager_ = new EventPropertyManager();
    addObservation(processorNetwork_);
    processorNetwork_->addObserver(this);
    buildLayout();
    currentIndex_ = 0;
    prevProcessorsWithInteractionHandlersSize_ = 0;
    processorsWithInteractionHandlers_ = new std::vector<Processor*>();
    findProcessorsWithInteractionHandlers(processorsWithInteractionHandlers_, processorNetwork_->getProcessors());
    updateWidget();
}

MappingWidget::~MappingWidget() {
    delete processorsWithInteractionHandlers_;
    delete eventPropertyManager_;
}

void MappingWidget::buildLayout() {
    // Components needed for layout
    QFrame* frame_ = new QFrame();
    comboBox_ = new QComboBox();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QVBoxLayout* topLayout = new QVBoxLayout();
    QScrollArea* scrollArea = new QScrollArea();
    EventPropertyManagerWidget* eventPropertyManagerWidget_ = new EventPropertyManagerWidget(eventPropertyManager_);
    // mainLayout contains topLayout and scrollArea.
    // scrollArea contains a widget with the botLayout.
    // Eventpropertywidgets will be added to the botlayout.
    topLayout->addWidget(comboBox_);
    scrollArea->setWidget(eventPropertyManagerWidget_);
    scrollArea->setWidgetResizable(true);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(scrollArea);
    frame_->setLayout(mainLayout);
    setWidget(frame_);
    QObject::connect(comboBox_, SIGNAL(activated(int)), this, SLOT(comboBoxChange()));
}

void MappingWidget::onProcessorNetworkChange() {
    findProcessorsWithInteractionHandlers(processorsWithInteractionHandlers_, processorNetwork_->getProcessors());

    if (processorsWithInteractionHandlers_->size() != prevProcessorsWithInteractionHandlersSize_)
        updateWidget();
}

void MappingWidget::updateWidget() {
    if (comboBox_->count() > 0)
        currentIndex_ =  comboBox_->currentIndex();

    comboBox_->clear();
    std::vector<EventProperty*> eventProperties, tmp;
    const std::vector<InteractionHandler*>& interactionHandlers;
    std::map<std::string, std::vector<EventProperty*> >* eventPropertyMap = eventPropertyManager_->getEventPropertyMap();
    PropertyOwner* eventPropertyOwner;

    // Get all eventproperties from the processornetwork
    for (size_t i = 0; i < processorsWithInteractionHandlers_->size(); ++i) {
        interactionHandlers = processorsWithInteractionHandlers_->at(i)->getInteractionHandlers();

        for (size_t j = 0; j < interactionHandlers.size(); ++j) {
            eventPropertyOwner = dynamic_cast<PropertyOwner*>(interactionHandlers.at(j));

            if (eventPropertyOwner) { /* Check if interactionhandlar has properties */
                tmp = eventPropertyOwner->getPropertiesByType<EventProperty>();
                eventProperties.insert(eventProperties.end(), tmp.begin(), tmp.end());
            }
        }

        // Add vector of eventproperties to map with processor identifier as key
        eventPropertyMap->insert(std::pair<std::string, std::vector<EventProperty*> >(processorsWithInteractionHandlers_->at(i)->getIdentifier(), eventProperties));
        comboBox_->addItem(processorsWithInteractionHandlers_->at(i)->getIdentifier().c_str());
        eventProperties.clear();
    }

    if (currentIndex_ > comboBox_->count()-1) currentIndex_ = 0;

    // Set selected processor and draw eventpropertywidgets
    if (comboBox_->count() > 0) {
        comboBox_->setCurrentIndex(currentIndex_);
        std::string identifier = (const char*)comboBox_->currentText().toLatin1();
        eventPropertyManager_->setActiveProcessor(identifier.c_str());
    } else
        eventPropertyManager_->notifyEventPropertyManagerObservers();

    prevProcessorsWithInteractionHandlersSize_ = processorsWithInteractionHandlers_->size();
}

void MappingWidget::comboBoxChange() {
    std::string identifier = (const char*)comboBox_->currentText().toLatin1();
    eventPropertyManager_->setActiveProcessor(identifier.c_str());
}

void MappingWidget::findProcessorsWithInteractionHandlers(std::vector<Processor*>* container, std::vector<Processor*> processors) {
    container->clear();

    for (size_t i = 0; i < processors.size(); ++i) {
        if (processors.at(i)->hasInteractionHandler())
            container->push_back(processors.at(i));
    }
}

} // namespace