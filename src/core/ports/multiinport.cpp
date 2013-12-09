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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#include <inviwo/core/ports/multiinport.h>

namespace inviwo {

MultiInport::MultiInport(std::string identifier)
    : Inport(identifier), inports_(new InportSet()) {
} 

void MultiInport::setProcessorHelper(Port* port, Processor* processor) { 
     port->setProcessor(processor); 
}

MultiInport::~MultiInport() {
    InportSet::iterator it = inports_->begin(); InportSet::iterator endIt = inports_->end();
    for(; it != endIt; ++it) {
        delete *it;
    }
    delete inports_; inports_ = NULL; 
}

PropertyOwner::InvalidationLevel MultiInport::getInvalidationLevel() const {
    InportSet::const_iterator it = inports_->begin(); InportSet::const_iterator endIt = inports_->end();
    PropertyOwner::InvalidationLevel maxInvalidationLevel(PropertyOwner::VALID);
    for(; it != endIt; ++it) {
        maxInvalidationLevel = std::max(maxInvalidationLevel, (*it)->getInvalidationLevel());
    }
    return maxInvalidationLevel;
}

void MultiInport::setInvalidationLevel( PropertyOwner::InvalidationLevel invalidationLevel ) {
    InportSet::iterator it = inports_->begin(); InportSet::iterator endIt = inports_->end();
    for(; it != endIt; ++it) {
        (*it)->setInvalidationLevel(invalidationLevel);
    }
}

bool MultiInport::isConnectedTo( Outport* outport ) const {
    InportSet::const_iterator it = inports_->begin(); InportSet::const_iterator endIt = inports_->end();
    for(; it != endIt; ++it) {
        if((*it)->isConnectedTo(outport)) {
            return true;
        }
    }
    return false;
}




std::vector<Outport*> MultiInport::getConnectedOutports() const {
    InportSet::const_iterator it = inports_->begin(); InportSet::const_iterator endIt = inports_->end();
    std::vector<Outport*> connectedOutports;
    for(; it != endIt; ++it) {
        std::vector<Outport*> inportConnectedOutports = (*it)->getConnectedOutports();
        std::vector<Outport*>::const_iterator outportIt = inportConnectedOutports.begin(); std::vector<Outport*>::const_iterator outportEndIt = inportConnectedOutports.end();
        for(; outportIt != outportEndIt; ++outportIt) {
            connectedOutports.push_back(*outportIt);
        }
        
    }
    return connectedOutports;
}

Outport* MultiInport::getConnectedOutport() const {
    if(isConnected()) {
        return (*(inports_->begin()))->getConnectedOutport();
    } else {
        return NULL;
    }
}

void MultiInport::invalidate( PropertyOwner::InvalidationLevel invalidationLevel ) {
    InportSet::iterator it = inports_->begin(); InportSet::iterator endIt = inports_->end();
    for(; it != endIt; ++it) {
        (*it)->invalidate(invalidationLevel);
    }
}

} // namespace
