#include <inviwo/core/interaction/pickinghandler.h>

namespace inviwo {

PickingHandler::PickingHandler(ImageOutport* imagePort) : InteractionHandler(),
      imagePort_(imagePort) {
}

PickingHandler::~PickingHandler() {}

void PickingHandler::invokeEvent(Event* event) {
}

void PickingHandler::serialize(IvwSerializer &s) const {
	InteractionHandler::serialize(s);
}

void PickingHandler::deserialize(IvwDeserializer &d) {
    InteractionHandler::deserialize(d);
}

} // namespace