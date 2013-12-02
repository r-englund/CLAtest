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

#ifndef IVW_INTERACTIONHANDLER_H
#define IVW_INTERACTIONHANDLER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/events/eventlistener.h>
#include <inviwo/core/io/serialization/ivwserializable.h>

namespace inviwo {

    class IVW_CORE_API InteractionHandler : public EventListener, public IvwSerializable {

    public:
        InteractionHandler();
        virtual ~InteractionHandler();

		virtual std::string getClassName() const { return "undefined"; }

		void serialize(IvwSerializer &s) const;
		void deserialize(IvwDeserializer &d);
    };

} // namespace

#endif // IVW_INTERACTIONHANDLER_H