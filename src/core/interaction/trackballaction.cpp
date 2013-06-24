#include <inviwo/core/interaction/trackballaction.h>

namespace inviwo {

TrackballAction::TrackballAction( TrackballAction::Actions action ) {
    actionNames_[TRACKBALL_ROTATE] = "Trackball rotate";
    actionNames_[TRACKBALL_ZOOM] = "Trackball zoom";
    actionNames_[TRACKBALL_PAN] = "Trackball pan";

    action_ = action;
    actionName_ = actionNames_[action];
}

TrackballAction::~TrackballAction() {}

void TrackballAction::serialize( IvwSerializer& s ) const {
	Action::serialize(s);
	s.serialize("action", actionName_);
}
void TrackballAction::deserialize(IvwDeserializer& d) {
	d.deserialize("action", actionName_);
	for (size_t i = 0; i < COUNT; ++i) {
		if (actionNames_[i] == actionName_) {
			action_ = i;
			break;
		}
	}
}

} //namespace