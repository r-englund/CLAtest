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

#ifndef IVW_INPORT_H
#define IVW_INPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/port.h>

namespace inviwo {

class Outport;

class IVW_CORE_API Inport : public Port {

public:
    Inport(std::string identifier);
    virtual ~Inport();

    //FIXME: Temporary fix. Remove this to make Inport abstract class
    virtual void initialize() {}
    virtual void deinitialize() {}

    virtual bool isConnected() const { return false; }
    virtual bool isReady() const { return isConnected(); }

    virtual void setInvalidationLevel(PropertyOwner::InvalidationLevel invalidationLevel) {};

    virtual bool canConnectTo(Port* port) const { return false; }
    virtual void connectTo(Outport* outport) {};
    virtual void disconnectFrom(Outport* outport) {};

    virtual bool isConnectedTo(Outport* outport) const { return false; }
    
    virtual Outport* getConnectedOutport() const { return NULL; }
    virtual std::vector<Outport*> getConnectedOutports() const { return std::vector<Outport*>(); }

    std::vector<Processor*> getPredecessors();

protected:
    template <typename T>
    void getPredecessorsUsingPortType(std::vector<Processor*>&);

};

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

#endif // IVW_INPORT_H