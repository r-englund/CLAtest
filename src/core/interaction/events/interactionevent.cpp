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
 * Primary author : Hans-Christian Helltegen
 *
 **********************************************************************/

#include <inviwo/core/interaction/events/interactionevent.h>

namespace inviwo {

InteractionEvent::InteractionEvent() {
    modifierNames_[MODIFIER_NONE] = "";
    modifierNames_[MODIFIER_ALT] = "Alt";
    modifierNames_[MODIFIER_CTRL] = "Ctrl";
    modifierNames_[MODIFIER_SHIFT] = "Shift";
}
InteractionEvent::~InteractionEvent() {}

void InteractionEvent::serialize(IvwSerializer& s) const {
	s.serialize("type", getClassName(), true);
	s.serialize("modifier", modifierName_);
}

void InteractionEvent::deserialize(IvwDeserializer& d) {
    std::string className;
    d.deserialize("type", className, true);
	d.deserialize("modifier", modifierName_);
	for (size_t i = 0; i < COUNT; ++i) {
		if (modifierNames_[i] == modifierName_) {
			modifier_ = static_cast<InteractionEvent::Modifier>(i);
			break;
		}
	}
}

} // namespace