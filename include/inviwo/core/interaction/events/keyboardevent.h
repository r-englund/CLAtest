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

#ifndef IVW_KEYBOARDEVENT_H
#define IVW_KEYBOARDEVENT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/events/interactionevent.h>

namespace inviwo {

class IVW_CORE_API KeyboardEvent : public InteractionEvent {
public:
	enum KeyState {
		KEY_STATE_NONE = 0,
		KEY_STATE_PRESS,
		KEY_STATE_RELEASE
	};

	KeyboardEvent();
    KeyboardEvent(char ascii, InteractionEvent::Modifier modifier=InteractionEvent::MODIFIER_NONE, KeyboardEvent::KeyState state=KeyboardEvent::KEY_STATE_PRESS);
    ~KeyboardEvent();

	inline KeyboardEvent::KeyState state() const { return state_; }

	virtual std::string getClassName() const { return "KeyboardEvent"; }
	virtual void serialize(IvwSerializer& s) const;
	virtual void deserialize(IvwDeserializer& d);

private:
	KeyboardEvent::KeyState state_;
};

} // namespace

#endif // IVW_KEYBOARDEVENT_H