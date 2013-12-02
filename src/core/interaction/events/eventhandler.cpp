/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/core/interaction/events/eventhandler.h>
#include <algorithm>

namespace inviwo {

EventHandler::EventHandler() {
    eventListeners_.clear();
}

EventHandler::~EventHandler() {
}

void EventHandler::broadcast(Event* event) {
    for (size_t i=0; i<eventListeners_.size(); i++)
        eventListeners_[i]->invokeEvent(event);
}

bool EventHandler::addEventListener(EventListener* listener){
    std::vector<EventListener*>::iterator it = std::find(eventListeners_.begin(), eventListeners_.end(), listener);
    if(it == eventListeners_.end()){
        eventListeners_.push_back(listener);
        return true;
    }
    return false;
}

bool EventHandler::removeEventListener(EventListener* listener){
    std::vector<EventListener*>::iterator it = std::find(eventListeners_.begin(), eventListeners_.end(), listener);
    if(it != eventListeners_.end()){
        eventListeners_.erase(it);
        return true;
    }
    return false;
}

} // namespace