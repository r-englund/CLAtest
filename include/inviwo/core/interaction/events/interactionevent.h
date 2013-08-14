#ifndef IVW_INTERACTIONEVENT_H
#define IVW_INTERACTIONEVENT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/serialization/ivwserializable.h>
#include <inviwo/core/interaction/events/event.h>

namespace inviwo {

class IVW_CORE_API InteractionEvent : public Event {
public:
    enum Modifier {
        MODIFIER_NONE  =      0,
        MODIFIER_ALT,
        MODIFIER_CTRL,
        MODIFIER_SHIFT,
        COUNT
    };

    InteractionEvent();
    virtual ~InteractionEvent();

    inline int button() const { return button_; }
    inline InteractionEvent::Modifier modifier() const { return modifier_; }
    inline std::string modifierName() const { return modifierName_; }
    inline std::string buttonName() const { return buttonName_; }

	virtual std::string getClassName() const { return "Undefined"; }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:
    InteractionEvent::Modifier modifier_;
    int button_;
    std::string modifierName_;
    std::string buttonName_;
    std::string modifierNames_[COUNT];
};

} // namespace

#endif // IVW_INTERACTIONEVENT_H