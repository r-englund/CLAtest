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

#include <inviwo/core/interaction/events/mouseevent.h>

namespace inviwo {

MouseEvent::MouseEvent(ivec2 position, MouseEvent::MouseButton button,
                       MouseEvent::MouseState state, InteractionEvent::Modifier modifier, uvec2 canvasSize)
    : InteractionEvent(),
      position_(position),
      state_(state),
      canvasSize_(canvasSize) {
          buttonNames_[MOUSE_BUTTON_LEFT] = "Left mouse button";
          buttonNames_[MOUSE_BUTTON_RIGHT] = "Right mouse button";
          buttonNames_[MOUSE_BUTTON_MIDDLE] = "Middle mouse button";
		  buttonNames_[MOUSE_BUTTON_NONE] = "";

          modifierName_ = modifierNames_[modifier];
          modifier_ = modifier;
          buttonName_ = buttonNames_[button];              
          button_ = button;
}

MouseEvent::MouseEvent( MouseEvent::MouseButton button, InteractionEvent::Modifier modifier )
    : InteractionEvent() {
    buttonNames_[MOUSE_BUTTON_LEFT] = "Left mouse button";
    buttonNames_[MOUSE_BUTTON_RIGHT] = "Right mouse button";
    buttonNames_[MOUSE_BUTTON_MIDDLE] = "Middle mouse button";
	buttonNames_[MOUSE_BUTTON_NONE] = "";

    modifierName_ = modifierNames_[modifier];
    modifier_ = modifier;
    buttonName_ = buttonNames_[button];              
    button_ = button;
}

MouseEvent::~MouseEvent() {}

void MouseEvent::serialize( IvwSerializer& s ) const {
	InteractionEvent::serialize(s);
	s.serialize("button", buttonName_);
}

void MouseEvent::deserialize( IvwDeserializer& d ) {
	InteractionEvent::deserialize(d);
	d.deserialize("button", buttonName_);
	for (int i = 0; i < COUNT; ++i) {
		if (buttonNames_[i] == buttonName_) {
			button_ = i;
			break;
		}
	}
}

} // namespace