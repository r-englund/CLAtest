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

#include <inviwo/core/interaction/pickinghandler.h>

namespace inviwo {

PickingHandler::PickingHandler(ImageOutport* imagePort) : InteractionHandler(),
      imagePort_(imagePort) {
}

PickingHandler::~PickingHandler() {}

void PickingHandler::invokeEvent(Event* event) {
}

void PickingHandler::serialize(IvwSerializer &s) const {
	InteractionHandler::serialize(s);
}

void PickingHandler::deserialize(IvwDeserializer &d) {
    InteractionHandler::deserialize(d);
}

} // namespace