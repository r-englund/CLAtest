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
#include <inviwo/core/util/callback.h>

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
    
    virtual std::string getClassName() const {return "Inport";}

    template <typename T>
    void onChange(T* o, void (T::*m)()) {
        onChangeCallback_.addMemberFunction(o,m);
    }
    void callOnChangeIfInvalid();
    
protected:
    template <typename T>
    void getPredecessorsUsingPortType(std::vector<Processor*>&);

    CallBackList onChangeCallback_;

};

} // namespace

#endif // IVW_INPORT_H