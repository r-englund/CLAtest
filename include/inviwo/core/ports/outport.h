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

#ifndef IVW_OUTPORT_H
#define IVW_OUTPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/port.h>

namespace inviwo {

class SingleInport;
class MultiInport;
class Inport;


class IVW_CORE_API Outport : public Port {

friend class SingleInport;
friend class MultiInport;
public:
    Outport(std::string identifier,
            PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~Outport();

    //TODO: Temporary fix to enable Deserialization. Remove this to make Outport abstract class
    virtual void initialize() {}
    virtual void deinitialize() {}

    virtual bool isConnected() const;
    bool isConnectedTo(Inport* port) const;

    virtual bool isReady() const { return isConnected(); }

    std::vector<Inport*> getConnectedInports() const { return connectedInports_; }

    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);
    virtual PropertyOwner::InvalidationLevel getInvalidationLevel() const { return invalidationLevel_; } 
    virtual void setInvalidationLevel(PropertyOwner::InvalidationLevel invalidationLevel);

    std::vector<Processor*> getDirectSuccessors();

protected:
    void connectTo(Inport* port);
    void disconnectFrom(Inport* port);
    
    template <typename T>
    void getSuccessorsUsingPortType(std::vector<Processor*>&);

    PropertyOwner::InvalidationLevel invalidationLevel_;
private:
    std::vector<Inport*> connectedInports_;
};

} // namespace

#endif // IVW_OUTPORT_H