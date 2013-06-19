#include <inviwo/core/interaction/interactionhandler.h>

namespace inviwo {

    InteractionHandler::InteractionHandler() : EventListener() {}
    InteractionHandler::~InteractionHandler() {}

	void InteractionHandler::serialize(IvwSerializer &s) const {
		s.serialize("type", getClassName(), true);
	}

	void InteractionHandler::deserialize(IvwDeserializer &d) {
        IVW_UNUSED_PARAM(d);  
    }

} // namespace
