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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include <inviwo/core/ports/singleinport.h>

#include <inviwo/core/processors/processor.h>

namespace inviwo {

SingleInport::SingleInport(std::string identifier,
                           PropertyOwner::InvalidationLevel invalidationLevel)
    : Inport(identifier)
    , connectedOutport_(NULL)
    , invalidationLevel_(invalidationLevel){
}

SingleInport::~SingleInport() {}

//Inport should determine if we can connect to the outport
void SingleInport::connectTo(Outport* outport) {
    connectedOutport_ = outport;
    outport->connectTo(this);
    invalidate(invalidationLevel_);
}

void SingleInport::disconnectFrom(Outport* outport) {
    ivwAssert(connectedOutport_==outport, "Ports are not connected.");
    if(outport == connectedOutport_) {
        connectedOutport_ = NULL;
        outport->disconnectFrom(this);
        invalidate(invalidationLevel_);
    }
}

bool SingleInport::isConnected() const { 
    return (connectedOutport_!=NULL); 
}

bool SingleInport::isConnectedTo(Outport* outport) const {
    return connectedOutport_==outport;
}

void SingleInport::invalidate(PropertyOwner::InvalidationLevel invalidationLevel) {
    invalidationLevel_ = std::max(invalidationLevel_, invalidationLevel);
    //TODO: for port properties Port::invalidate() should be called here
    Port::invalidate(invalidationLevel);
}

} // namespace
