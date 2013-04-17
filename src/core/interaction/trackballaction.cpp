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

} //namespace