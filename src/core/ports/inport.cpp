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

#include <inviwo/core/ports/inport.h>

#include <inviwo/core/processors/processor.h>

namespace inviwo {

Inport::Inport(std::string identifier,
               PropertyOwner::InvalidationLevel invalidationLevel)
: Port(identifier, invalidationLevel), connectedOutport_(NULL)
{}

Inport::~Inport() {}

//Inport should determine if we can connect to the outport
void Inport::connectTo(Outport* outport) {
    connectedOutport_ = outport;
    outport->connectTo(this);
    invalidate(invalidationLevel_);
}

void Inport::disconnectFrom(Outport* outport) {
    // TODO: check if ports are connected
    ivwAssert(connectedOutport_==outport, "Ports are not connected.");
    connectedOutport_ = NULL;
    outport->disconnectFrom(this);
    invalidate(invalidationLevel_);
}

bool Inport::isConnected() const { 
    return (connectedOutport_!=NULL); 
}

bool Inport::isConnectedTo(Outport* outport) const {
    return connectedOutport_==outport;
}

void Inport::invalidate(PropertyOwner::InvalidationLevel invalidationLevel) {
    Port::invalidate(invalidationLevel);
}

template <typename T>
void Inport::getPredecessorsUsingPortType(std::vector<Processor*>& predecessorsProcessors) {
    Port* connectedOutport = dynamic_cast<Port*>(connectedOutport_);
    if (connectedOutport) {
        Processor* predecessorsProcessor = connectedOutport->getProcessor();

        if (std::find(predecessorsProcessors.begin(), predecessorsProcessors.end(), predecessorsProcessor)== predecessorsProcessors.end())
            predecessorsProcessors.push_back(predecessorsProcessor);

        std::vector<Inport*> inports = predecessorsProcessor->getInports();
        for (size_t j=0; j<inports.size(); j++) {
            T* inPort = dynamic_cast<T*>(inports[j]);
            if (inPort)
                inPort->template getPredecessorsUsingPortType<T>(predecessorsProcessors);
        }
    }
}

std::vector<Processor*> Inport::getPredecessors() {
    std::vector<Processor*> predecessorsProcessors;
    getPredecessorsUsingPortType<Inport>(predecessorsProcessors);
    return predecessorsProcessors;
}

} // namespace
