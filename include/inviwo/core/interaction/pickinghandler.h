#ifndef IVW_PICKINGHANDLER_H
#define IVW_PICKINGHANDLER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/interactionhandler.h>
#include <inviwo/core/interaction/events/event.h>
#include <inviwo/core/io/serialization/ivwserializable.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/core/ports/imageport.h>

namespace inviwo {

class IVW_CORE_API PickingHandler : public InteractionHandler {

public:
    PickingHandler(ImageOutport*);
    ~PickingHandler();

    virtual void invokeEvent(Event* event);

	virtual std::string getClassName() const { return "PickingHandler"; }

	void serialize(IvwSerializer &s) const;
	void deserialize(IvwDeserializer &d);

private:
    ImageOutport* imagePort_;

    EventProperty* pickingEventProperty_;
};

} // namespace

#endif // IVW_PICKINGHANDLER_H