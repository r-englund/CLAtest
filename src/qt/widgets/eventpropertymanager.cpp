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

#include <inviwo/qt/widgets/eventpropertymanager.h>

namespace inviwo {

EventPropertyManager::EventPropertyManager() : EventPropertyManagerObservable() {}
EventPropertyManager::~EventPropertyManager() {}

// Remap with a mouse event
void EventPropertyManager::changeMouseMapping(EventProperty* eventProperty, MouseEvent::MouseButton button,
        InteractionEvent::Modifier modifier) {
    if (dynamic_cast<MouseEvent*>(eventProperty->getEvent())) {
        
        if(eventPropertyMap_.empty())
            return;

        // Look for event conflicts
        std::vector<EventProperty*> eventProperties = eventPropertyMap_[activeProcessor_];

        for (size_t i = 0; i < eventProperties.size(); ++i) {
            if (eventProperties.at(i)->getEvent()->button() == button && eventProperties.at(i)->getEvent()->modifier() == modifier)
                eventProperties.at(i)->setEvent(new MouseEvent(MouseEvent::MOUSE_BUTTON_NONE, InteractionEvent::MODIFIER_NONE));
        }

        // Do the remapping
        for (size_t i = 0; i < eventProperties.size(); ++i) {
            if (eventProperty->getIdentifier() == eventProperties.at(i)->getIdentifier()) {
                eventProperties.at(i)->setEvent(new MouseEvent(button, modifier));
                break;
            }
        }
    } else
        LogWarn("Can only be bound with a keyboard event");

    notifyEventPropertyManagerObservers();
}

// Remap with a keyboard event
void EventPropertyManager::changeKeyMapping(EventProperty* eventProperty, char button, InteractionEvent::Modifier modifier) {
    if (dynamic_cast<KeyboardEvent*>(eventProperty->getEvent())) {
        // Look for event conflicts
        std::vector<EventProperty*> eventProperties = eventPropertyMap_[activeProcessor_];

        for (size_t i = 0; i < eventProperties.size(); ++i) {
            if (eventProperties.at(i)->getEvent()->button() == button && eventProperties.at(i)->getEvent()->modifier() == modifier)
                eventProperties.at(i)->setEvent(new KeyboardEvent());
        }

        // Do the remapping
        for (size_t i = 0; i < eventProperties.size(); ++i) {
            if (eventProperty->getIdentifier() == eventProperties.at(i)->getIdentifier()) {
                eventProperties.at(i)->setEvent(new KeyboardEvent(button, modifier, KeyboardEvent::KEY_STATE_PRESS));
                break;
            }
        }
    } else
        LogWarn("Can only be bound with a mouse event");

    notifyEventPropertyManagerObservers();
}

std::vector<EventProperty*> EventPropertyManager::getEventPropertiesFromMap() {
    std::map<std::string, std::vector<EventProperty*> >::iterator it;
    it = eventPropertyMap_.find(activeProcessor_);
    return it->second;
}

void EventPropertyManager::setActiveProcessor(std::string processorIdentifier) {
    activeProcessor_ = processorIdentifier;
    //notifyEventPropertyManagerObservers();
}

bool EventPropertyManager::isEmpty() {
    if (eventPropertyMap_.size() > 0)
        return false;
    else
        return true;
}

} // namespace