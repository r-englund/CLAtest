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
    Inport(std::string identifier,
           PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~Inport();

    //FIXME: Temporary fix. Remove this to make Inport abstract class
    virtual void initialize() {}
    virtual void deinitialize() {}

    virtual bool canConnectTo(Port* port) const { return false; };
    virtual void connectTo(Outport* outport);
    virtual void disconnectFrom(Outport* outport);

    virtual bool isConnected() const;
    bool isConnectedTo(Outport* outport) const;

    virtual bool isReady() const { return isConnected(); }

    Outport* getConnectedOutport() const { return connectedOutport_; }

    void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);
    virtual std::vector<Processor*> getPredecessors();

protected:
    template <typename T>
    void getPredecessorsUsingPortType(std::vector<Processor*>&);

private:
    Outport* connectedOutport_;
};

} // namespace

#endif // IVW_INPORT_H