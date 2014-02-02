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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_PORTCONNECTION_H
#define IVW_PORTCONNECTION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/inport.h>
#include <inviwo/core/ports/outport.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

class IVW_CORE_API PortConnection : public IvwSerializable {

public:
    PortConnection();
    PortConnection(Outport* outport, Inport* inport);
    virtual ~PortConnection();

    Inport* getInport() const { return inport_; }
    Outport* getOutport() const { return outport_; }

    bool involvesProcessor(Processor* processor) const {
        return (inport_->getProcessor()==processor ||
                outport_->getProcessor()==processor);
    }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

private:
    Inport* inport_;
    Outport* outport_;
};

} // namespace

#endif // IVW_PORTCONNECTION_H
