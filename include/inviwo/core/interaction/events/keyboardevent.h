#ifndef IVW_KEYBOARDEVENT_H
#define IVW_KEYBOARDEVENT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/events/event.h>

namespace inviwo {

class IVW_CORE_API KeyboardEvent : public Event {
public:
    KeyboardEvent(char ascii, Event::Modifier modifier);
    ~KeyboardEvent();

	virtual std::string getClassName() const { return "KeyboardEvent"; }

	virtual void serialize(IvwSerializer& s) const;
	virtual void deserialize(IvwDeserializer& d);
};

} // namespace

#endif // IVW_KEYBOARDEVENT_H