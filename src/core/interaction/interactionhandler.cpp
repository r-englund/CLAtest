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
 * Primary author : Hans-Christian Helltegen
 *
 **********************************************************************/

#include <inviwo/core/interaction/interactionhandler.h>

namespace inviwo {

InteractionHandler::InteractionHandler() : EventListener() {}
InteractionHandler::~InteractionHandler() {}

void InteractionHandler::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassName(), true);
}

void InteractionHandler::deserialize(IvwDeserializer& d) {
    std::string className;
    d.deserialize("type", className, true);
}

} // namespace
