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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_PICKINGHANDLER_H
#define IVW_PICKINGHANDLER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/interactionhandler.h>
#include <inviwo/core/interaction/events/event.h>
#include <inviwo/core/io/serialization/ivwserializable.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/core/ports/imageport.h>

namespace inviwo {

class IVW_CORE_API PickingHandler : public InteractionHandler {

public:
    PickingHandler(ImageOutport*);
    ~PickingHandler();

    virtual void invokeEvent(Event* event);

	virtual std::string getClassName() const { return "PickingHandler"; }

	void serialize(IvwSerializer &s) const;
	void deserialize(IvwDeserializer &d);

private:
    ImageOutport* imagePort_;

    EventProperty* pickingEventProperty_;
};

} // namespace

#endif // IVW_PICKINGHANDLER_H