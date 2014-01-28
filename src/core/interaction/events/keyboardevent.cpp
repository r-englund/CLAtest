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

#include <inviwo/core/interaction/events/keyboardevent.h>

namespace inviwo {

KeyboardEvent::KeyboardEvent(char ascii, InteractionEvent::Modifier modifier, KeyboardEvent::KeyState state) : InteractionEvent() {
	state_ = state;
	button_ = ascii;
	buttonName_ = ascii;
	modifier_ = modifier;
	modifierName_ = modifierNames_[modifier_];	
}

KeyboardEvent::KeyboardEvent() {
	buttonName_ = "";
}

KeyboardEvent::~KeyboardEvent() {}

void KeyboardEvent::serialize(IvwSerializer& s) const {
	InteractionEvent::serialize(s);
	s.serialize("button", buttonName_);
}

void KeyboardEvent::deserialize(IvwDeserializer& s) {
	InteractionEvent::deserialize(s);
	s.deserialize("button", buttonName_);
	button_ = buttonName_[0];
}

} // namespace