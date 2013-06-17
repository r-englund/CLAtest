#include <inviwo/core/interaction/interactionhandler.h>

namespace inviwo {

    InteractionHandler::InteractionHandler() : EventListener() {}
    InteractionHandler::~InteractionHandler() {}

	void InteractionHandler::serialize( IvwSerializer &s ) const {}
	void InteractionHandler::deserialize( IvwDeserializer &s ) {}

} // namespace
