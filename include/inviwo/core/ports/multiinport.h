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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_MULTI_INPORT_H
#define IVW_MULTI_INPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/inport.h>
#include <set>

namespace inviwo {

/** \class MultiInport
*
* Support class for inports that can handle multiple inputs.
* This class allows us to check if an Inport
* can handle multiple connections through dynamic_cast.
* @see MultiDataInport
*/
class IVW_CORE_API MultiInport : public Inport {
public:
    typedef std::set< Inport* > InportSet;
    MultiInport(std::string identifier);
    virtual ~MultiInport();

    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);

    bool isConnected() const { return !inports_->empty(); }

    bool isConnectedTo(Outport* outport) const;

    virtual bool isReady() const { return isConnected(); }

    virtual PropertyOwner::InvalidationLevel getInvalidationLevel() const;
    virtual void setInvalidationLevel(PropertyOwner::InvalidationLevel invalidationLevel);

    virtual Outport* getConnectedOutport() const;
    std::vector<Outport*> getConnectedOutports() const;
    size_t getNumConnectedOutports() const { return inports_->size(); }

    void disconnectFrom(Outport* outport) {
        InportSet::iterator it;

        for (it = inports_->begin(); it != inports_->end(); ++it) {
            // Find connected port
            if ((*it)->isConnectedTo(outport)) {
                (*it)->disconnectFrom(outport);
                delete *it;
                inports_->erase(it);
                break;
            }
        }
    }

protected:

    /**
     * Derived classes of Port will not have access to Port::setProcessor.
     * To make it simeple and avoid template nightmares
     * this class is instead friend of Port. This method
     * allows MultiDataInport<T,U> classes
     * to call setProcessor on a port through this helper function.
     * This method simply calls port->setProcessor
     * @param port
     * @param processor
     */
    void setProcessorHelper(Port* port, Processor* processor);

    InportSet* inports_;
};

} // namespace

#endif // IVW_MULTI_INPORT_H