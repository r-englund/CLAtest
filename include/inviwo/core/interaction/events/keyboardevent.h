#ifndef IVW_KEYBOARDEVENT_H
#define IVW_KEYBOARDEVENT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/events/event.h>

namespace inviwo {

class IVW_CORE_API KeyboardEvent : public Event {
public:
	enum KeyState {
		KEY_STATE_NONE = 0,
		KEY_STATE_PRESS,
		KEY_STATE_RELEASE
	};

	KeyboardEvent();
    KeyboardEvent(char ascii, Event::Modifier modifier=Event::MODIFIER_NONE, KeyboardEvent::KeyState state=KeyboardEvent::KEY_STATE_PRESS);
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