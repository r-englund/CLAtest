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

Inport::Inport(std::string identifier)
: Port(identifier)
{}

Inport::~Inport() {}

std::vector<Processor*> Inport::getPredecessors() {
    std::vector<Processor*> predecessorsProcessors;
    getPredecessorsUsingPortType<Inport>(predecessorsProcessors);
    return predecessorsProcessors;
}

template <typename T>
void Inport::getPredecessorsUsingPortType(std::vector<Processor*>& predecessorsProcessors) {
    if (isConnected()) {

        std::vector<Outport*> connectedOutports = getConnectedOutports();
        std::vector<Outport*>::const_iterator it = connectedOutports.begin(); std::vector<Outport*>::const_iterator endIt = connectedOutports.end();
        for(; it != endIt; ++it) {
            Processor* predecessorsProcessor = (*it)->getProcessor();

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
}




} // namespace
