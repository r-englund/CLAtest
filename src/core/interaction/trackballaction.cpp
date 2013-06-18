#include <inviwo/core/interaction/trackballaction.h>

namespace inviwo {

TrackballAction::TrackballAction( TrackballAction::Action action ) {
    actionNames_[TRACKBALL_ROTATE] = "Trackball rotate";
    actionNames_[TRACKBALL_ZOOM] = "Trackball zoom";
    actionNames_[TRACKBALL_PAN] = "Trackball pan";

    action_ = action;
    actionName_ = actionNames_[action];
}

TrackballAction::~TrackballAction() {}

void TrackballAction::serialize( IvwSerializer& s ) const {
	s.serialize("action", action_);
}
void TrackballAction::deserialize( IvwDeserializer& s ) {
	s.deserialize("action", action_);
	actionName_ = actionNames_[action_];
}

} //namespace