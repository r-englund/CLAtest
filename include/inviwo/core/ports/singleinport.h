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

#ifndef IVW_SINGLE_INPORT_H
#define IVW_SINGLE_INPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/inport.h>

namespace inviwo {

class Outport;

class IVW_CORE_API SingleInport : public Inport {

public:
    /**
     * @param invalidationLevel Defines the level of invalidation used upon connection/deconnection.
     * @see Processor::addPort(), PropertyOwner::InvalidationLevel
     */
    SingleInport(std::string identifier,
           PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~SingleInport();

    //FIXME: Temporary fix. Remove this to make SingleInport abstract class
    virtual void initialize() {}
    virtual void deinitialize() {}
    virtual PropertyOwner::InvalidationLevel getInvalidationLevel() const { return invalidationLevel_; }

    virtual void setInvalidationLevel(PropertyOwner::InvalidationLevel invalidationLevel) {
        invalidationLevel_ = invalidationLevel;
    }

    virtual void connectTo(Outport* outport);
    virtual void disconnectFrom(Outport* outport);

    virtual bool isConnected() const;
    bool isConnectedTo(Outport* outport) const;

    virtual bool isReady() const { return isConnected(); }

    Outport* getConnectedOutport() const { return connectedOutport_; }
    std::vector<Outport*> getConnectedOutports() const { return std::vector<Outport*>(1, connectedOutport_); }

    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);
protected:
    Outport* connectedOutport_;
    PropertyOwner::InvalidationLevel invalidationLevel_;
};



} // namespace

#endif // IVW_SINGLE_INPORT_H